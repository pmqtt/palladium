#ifndef PALLADIUM_VM_MEMORY_H_
#define PALLADIUM_VM_MEMORY_H_
#include <array>
#include <bitset>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <sys/mman.h>
#include <sys/types.h>
#include <tuple>
#include <vector>

// Memory Layout for Segments and Blocks
// -------------------------------------
// [0 Byte             SEGMENT                  128 Byte] ..... [SEGMENT]
// |  32 Blocks (1 Byte)  |  8 Blocks (4 Byte)  |  4 Blocks (8 Byte)  |
// |  2 Blocks (16 Byte)
// --------------------------------------------------------------
// [0]   ...  [31]       [32..35]          ...          [59..63]
//
// - Each segment is 128 bytes in size.
// - Blocks within a segment are divided into sub-blocks of varying sizes:
//     - 32 blocks of 1 byte each.
//     - 8 blocks of 4 bytes each.
//     - 4 blocks of 8 bytes each.
//     - 2 blocks of 16 bytes each.
// - The indices in square brackets ([...]) denote the memory range for each
// set of blocks.

struct VMMemorySegment {
  static constexpr std::size_t ONE_BYTE_BLOCK_START = 0;
  static constexpr std::size_t ONE_BYTE_BLOCK_END = 31;
  static constexpr std::size_t FOUR_BYTE_BLOCK_START = 32;
  static constexpr std::size_t FOUR_BYTE_BLOCK_END = 39;
  static constexpr std::size_t EIGHT_BYTE_BLOCK_START = 40;
  static constexpr std::size_t EIGHT_BYTE_BLOCK_END = 43;
  static constexpr std::size_t SIXTEEN_BYTE_BLOCK_START = 44;
  static constexpr std::size_t SIXTEEN_BYTE_BLOCK_END = 45;

  char* start_adr;
  std::bitset<46> sub_blocks_free_list;
  std::array<std::pair<std::size_t, std::size_t>, 46> free_list;
};

template <class VM, std::size_t SSIZE = 128> struct VMMemory {
  static constexpr std::size_t SEGMENT_SIZE = SSIZE;

  VMMemory(std::size_t capacity) : _capacity(capacity) {
    _segment_count = _capacity / SEGMENT_SIZE;
    _base = static_cast<char*>(
        mmap(nullptr, _segment_count * SEGMENT_SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0));

    assert(_base != MAP_FAILED && "Allocation memory failed");

    for (std::size_t i = 0; i < _segment_count; ++i) {
      _segment_list.push_back({.start_adr = _base + (i * _segment_count), .sub_blocks_free_list = {}, .free_list = {}});
    }
  }

  auto allocate(std::size_t size) -> std::size_t {
    if (size <= 16) {
      return allocate_small_sub_block(size);
    }
    if (size <= SSIZE) {
      return allocate_sub_blocks(size);
    } else {
      return allocate_segments_and_blocks(size);
    }
  }

  void deallocate(std::size_t adr) {
    std::size_t base_adr = reinterpret_cast<std::size_t>(_base);
    VM::P::check_memory_adress(base_adr, adr);
    std::size_t segmentnr = (adr - base_adr) / SSIZE;
    auto& free_list = _segment_list[segmentnr].free_list;
    std::size_t sadr = reinterpret_cast<std::size_t>(_segment_list[segmentnr].start_adr);

    std::size_t index = (adr - sadr);
    auto limits = find_sub_block_by_index(index);
    bool found_adr = false;
    for (std::size_t i = limits.first; i <= limits.second; ++i) {
      if (free_list[i].first == adr) {
        index = i;
        found_adr = true;
        break;
      }
    }

    assert(found_adr && "Illigal memory access, adress not found");

    auto item = free_list[index];
    VM::P::check_equal_adress(adr, item.first);
    std::size_t total = 0;
    std::size_t tmp_index = index;
    while (total < item.second) {
      if (index >= free_list.size()) {
        index = 0;
        segmentnr++;
      }
      if (segmentnr >= _segment_list.size()) {
        assert(false && "Illigal free, not enough segments to free");
      }

      total += sizeof_sub_block(index);
      _segment_list[segmentnr].sub_blocks_free_list[index] = false;
      index++;
    }
    free_list[tmp_index] = {};
  }

  ~VMMemory() {
    munmap(_base, _capacity);
  }
  auto base() -> std::size_t {
    return reinterpret_cast<std::size_t>(_base);
  };
  auto segments() const -> const std::vector<VMMemorySegment>& {
    return _segment_list;
  }

private:
  auto allocate_small_sub_block(std::size_t size) -> std::size_t {
    if (size == 1) {
      return allocate_sub_block(VMMemorySegment::ONE_BYTE_BLOCK_START, VMMemorySegment::ONE_BYTE_BLOCK_END, 1);
    }
    if (size <= 4) {
      return allocate_sub_block(VMMemorySegment::FOUR_BYTE_BLOCK_START, VMMemorySegment::FOUR_BYTE_BLOCK_END, 4);
    }
    if (size <= 8) {
      return allocate_sub_block(VMMemorySegment::EIGHT_BYTE_BLOCK_START, VMMemorySegment::EIGHT_BYTE_BLOCK_END, 8);
    }
    if (size <= 16) {
      return allocate_sub_block(VMMemorySegment::SIXTEEN_BYTE_BLOCK_START, VMMemorySegment::SIXTEEN_BYTE_BLOCK_END, 16);
    }
    return 0;
  }

  auto allocate_sub_blocks(std::size_t size) -> std::size_t {
    for (auto& x : _segment_list) {
      std::int8_t index = find_free_start_index_in_segment(x, size);
      if (index >= 0) {
        std::uint8_t total = 0;
        std::size_t adr = reinterpret_cast<std::size_t>(x.start_adr + start_adr_of_sub_block(index));
        x.free_list[index] = std::make_tuple(adr, size);

        while (total < size) {
          total += sizeof_sub_block(index);
          x.sub_blocks_free_list[index] = true;

          index++;
        }
        return adr;
      }
    }
    return 0;
  }

  auto allocate_segments_and_blocks(std::size_t size) -> std::size_t {
    std::size_t amount_segments = size / SSIZE;
    std::size_t rest_size = size % SSIZE;
    for (auto iter = _segment_list.begin(); iter != _segment_list.end(); ++iter) {
      if (iter->sub_blocks_free_list.none()) {
        std::size_t adr = reinterpret_cast<std::size_t>(iter->start_adr);
        bool empty_segments = true;
        for (std::size_t i = 1; i < amount_segments; ++i) {
          ++iter;
          if (iter == _segment_list.end()) {
            return 0;
          }

          if (iter->sub_blocks_free_list.none() == false) {
            empty_segments = false;
            break;
          }
        }
        if (empty_segments == false) {
          continue;
        }
        if (rest_size == 0) {
          for (std::size_t i = 0; i < amount_segments; ++i) {
            mark_segment(*iter);
            --iter;
          }
          ++iter;
          (*iter).free_list[0] = std::make_tuple(adr, size);
          return adr;
        }

        ++iter;
        if (iter == _segment_list.end()) {
          return 0;
        }
        if (is_left_free(*iter, rest_size)) {
          mark_partial(*iter, rest_size);
          for (std::size_t i = 0; i < amount_segments; ++i) {
            --iter;
            mark_segment(*iter);
          }
          (*iter).free_list[0] = std::make_tuple(adr, size);
          return adr;
        }
      }
    }
    return 0;
  }

  auto allocate_sub_block(std::size_t start, std::size_t end, u_int8_t step) -> std::size_t {
    for (auto& segment : _segment_list) {
      for (std::size_t i = start; i <= end; ++i) {
        if (segment.sub_blocks_free_list[i] == false) {
          segment.sub_blocks_free_list[i] = true;
          std::uint8_t offset = start_adr_of_sub_block(start);
          offset += step * (i - start);
          char* adr = segment.start_adr + offset;
          segment.free_list[i] = std::make_tuple(reinterpret_cast<size_t>(adr), step);
          return reinterpret_cast<std::size_t>(adr);
        }
      }
    }
    return 0;
  }

  auto start_adr_of_sub_block(std::uint8_t sub_block_index) const -> std::uint8_t {
    if (sub_block_index <= VMMemorySegment::ONE_BYTE_BLOCK_END)
      return 0;
    if (sub_block_index <= VMMemorySegment::FOUR_BYTE_BLOCK_END)
      return 32;
    if (sub_block_index <= VMMemorySegment::EIGHT_BYTE_BLOCK_END)
      return 64;
    if (sub_block_index <= VMMemorySegment::SIXTEEN_BYTE_BLOCK_END)
      return 96;
    assert(false && "failed calculation of start adress in sub block");
  }

  auto find_sub_block_by_index(std::uint8_t index) const -> std::pair<std::size_t, std::size_t> {
    if (index < 32)
      return {VMMemorySegment::ONE_BYTE_BLOCK_START, VMMemorySegment::ONE_BYTE_BLOCK_END};
    if (index < 64)
      return {VMMemorySegment::FOUR_BYTE_BLOCK_START, VMMemorySegment::FOUR_BYTE_BLOCK_END};
    if (index < 96)
      return {VMMemorySegment::EIGHT_BYTE_BLOCK_START, VMMemorySegment::EIGHT_BYTE_BLOCK_END};
    return {VMMemorySegment::SIXTEEN_BYTE_BLOCK_START, VMMemorySegment::SIXTEEN_BYTE_BLOCK_END};
  }

  auto sizeof_sub_block(std::uint8_t index) const -> std::uint8_t {
    if (index <= VMMemorySegment::ONE_BYTE_BLOCK_END)
      return 1;
    if (index <= VMMemorySegment::FOUR_BYTE_BLOCK_END)
      return 4;
    if (index <= VMMemorySegment::EIGHT_BYTE_BLOCK_END)
      return 8;
    if (index <= VMMemorySegment::SIXTEEN_BYTE_BLOCK_END)
      return 16;
    assert(false && "failed calculation of size in sub block");
  }

  std::int8_t find_free_start_index_in_segment(const VMMemorySegment& segment, std::size_t sz) const {
    if (segment.sub_blocks_free_list.none()) {
      return 0;
    }
    std::size_t total = 0;
    std::int8_t adr = 0;
    for (std::uint8_t i = 0; i <= VMMemorySegment::SIXTEEN_BYTE_BLOCK_END; ++i) {
      if (segment.sub_blocks_free_list[i] == false) {
        total += sizeof_sub_block(i);
      }
      if (total >= sz) {
        return adr;
      }
      if (segment.sub_blocks_free_list[i] == true) {
        total = 0;
        adr = i + 1;
      }
    }
    return -1;
  }
  auto is_left_free(const VMMemorySegment& segment, std::uint8_t sz) -> bool {
    if (segment.sub_blocks_free_list.none()) {
      return true;
    }
    u_int8_t total = 0;
    uint8_t index = 0;
    while (total < sz && index <= VMMemorySegment::SIXTEEN_BYTE_BLOCK_END) {
      if (segment.sub_blocks_free_list[index] == false) {
        total += sizeof_sub_block(index);
        index += 1;
      } else {
        return false;
      }
    }
    return true;
  }

  void mark_partial(VMMemorySegment& segment, std::uint8_t sz) {
    u_int8_t total = 0;
    uint8_t index = 0;
    while (total < sz && index <= VMMemorySegment::SIXTEEN_BYTE_BLOCK_END) {
      total += sizeof_sub_block(index);
      segment.sub_blocks_free_list[index] = true;
      index += 1;
    }
  }

  void mark_segment(VMMemorySegment& segment) {
    segment.sub_blocks_free_list.set();
  }

private:
  std::size_t _capacity;
  char* _base;
  std::size_t _segment_count;
  std::vector<VMMemorySegment> _segment_list;
};

template <class VM, std::size_t SSIZE> std::ostream& operator<<(std::ostream& os, const VMMemory<VM, SSIZE>& memory) {
  os << "VMMemory State:" << std::endl;
  os << "----------------------------------------" << std::endl;

  std::size_t segment_index = 0;
  for (const auto& segment : memory.segments()) {
    os << "Segment " << segment_index++ << " [Start Address: 0x" << std::hex
       << reinterpret_cast<std::size_t>(segment.start_adr) << "]" << std::dec << std::endl;

    os << "  Blocks: ";
    for (std::size_t i = 0; i <= VMMemorySegment::SIXTEEN_BYTE_BLOCK_END; ++i) {
      if (segment.sub_blocks_free_list[i]) {
        os << "[X]"; // Belegter Block
      } else {
        os << "[ ]"; // Freier Block
      }
      if (i == VMMemorySegment::ONE_BYTE_BLOCK_END || i == VMMemorySegment::FOUR_BYTE_BLOCK_END ||
          i == VMMemorySegment::EIGHT_BYTE_BLOCK_END) {
        os << " | "; // Trenne die Blocktypen für Übersicht
      }
    }
    os << std::endl;
    os << " Data: ";
    for (std::size_t i = 0; i < 128; ++i) {
      os << *(segment.start_adr + i) << ";;";
    }
    os << std::endl;
  }

  os << "----------------------------------------" << std::endl;
  return os;
}

#endif
