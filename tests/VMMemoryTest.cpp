#include "VMMemory.h"
#include "VirtualMachine.h"
#include "purge.hpp"
PURGE_MAIN

SIMPLE_TEST_CASE(VMMemoryAllocaOneByteTest) {
  VMMemory<VirtualMachine<AggresivPolicy>> memory(128);
  for (std::size_t i = 0; i < 31; ++i) {
    std::size_t byte_adr1 = memory.allocate(1);
    REQUIRE(byte_adr1 == memory.base() + i);
  }
  std::size_t byte_adr = memory.allocate(1);
  REQUIRE(byte_adr == memory.base() + 31);
  REQUIRE(memory.allocate(1) == 0);
  memory.deallocate(byte_adr);
  std::size_t byte_adr2 = memory.allocate(1);
  REQUIRE(byte_adr == byte_adr2);
}

SIMPLE_TEST_CASE(VMMemoryAllocaFourByteTest) {
  VMMemory<VirtualMachine<AggresivPolicy>> memory(128);
  for (std::size_t i = 0; i < 7; ++i) {
    std::size_t byte_adr1 = memory.allocate(2);
    REQUIRE(byte_adr1 == memory.base() + 32 + 4 * i);
  }
  std::size_t byte_adr = memory.allocate(2);
  REQUIRE(byte_adr == memory.base() + 32 + 4 * 7);
  REQUIRE(memory.allocate(2) == 0);
  memory.deallocate(byte_adr);
  std::size_t byte_adr2 = memory.allocate(2);
  REQUIRE(byte_adr == byte_adr2);
}

SIMPLE_TEST_CASE(VMMemoryAllocaEightByteTest) {
  VMMemory<VirtualMachine<AggresivPolicy>> memory(128);
  for (std::size_t i = 0; i < 3; ++i) {
    std::size_t byte_adr1 = memory.allocate(5);
    REQUIRE(byte_adr1 == memory.base() + 64 + 8 * i);
  }
  std::size_t byte_adr = memory.allocate(5);
  REQUIRE(byte_adr == memory.base() + 64 + 8 * 3);
  REQUIRE(memory.allocate(5) == 0);
  memory.deallocate(byte_adr);
  std::size_t byte_adr2 = memory.allocate(5);
  REQUIRE(byte_adr == byte_adr2);
}

SIMPLE_TEST_CASE(VMMemoryAllocateSixteentByteTest) {
  VMMemory<VirtualMachine<AggresivPolicy>> memory(128);
  for (std::size_t i = 0; i < 1; ++i) {
    std::size_t byte_adr1 = memory.allocate(14);
    REQUIRE(byte_adr1 == memory.base() + 96 + 16 * i);
  }
  std::size_t byte_adr = memory.allocate(14);
  REQUIRE(byte_adr == memory.base() + 96 + 16 * 1);
  REQUIRE(memory.allocate(14) == 0);
  memory.deallocate(byte_adr);
  std::size_t byte_adr2 = memory.allocate(14);
  REQUIRE(byte_adr == byte_adr2);
}

SIMPLE_TEST_CASE(VMMemoryMixedAllocaTest) {
  VMMemory<VirtualMachine<AggresivPolicy>> memory(128);
  std::size_t adr1 = memory.allocate(1);  // 1 Byte
  std::size_t adr2 = memory.allocate(4);  // 4 Bytes
  std::size_t adr3 = memory.allocate(8);  // 8 Bytes
  std::size_t adr4 = memory.allocate(16); // 16 Bytes

  REQUIRE(adr1 == memory.base() + 0);
  REQUIRE(adr2 == memory.base() + 32);
  REQUIRE(adr3 == memory.base() + 64);
  REQUIRE(adr4 == memory.base() + 96);

  memory.deallocate(adr1);
  memory.deallocate(adr2);
  memory.deallocate(adr3);
  memory.deallocate(adr4);
  REQUIRE(memory.segments()[0].sub_blocks_free_list.none());
}

SIMPLE_TEST_CASE(VMMemoryLargeAllocaButInOneSegmentTest) {
  VMMemory<VirtualMachine<AggresivPolicy>> memory(128);
  memory.allocate(32);
  memory.allocate(32);
  memory.allocate(33);
  for (std::size_t i = 0; i < VMMemorySegment::EIGHT_BYTE_BLOCK_END; ++i) {
    REQUIRE(memory.segments().front().sub_blocks_free_list[i] == true);
  }
  REQUIRE(
      memory.segments()
          .front()
          .sub_blocks_free_list[VMMemorySegment::EIGHT_BYTE_BLOCK_END + 1] ==
      true);
  REQUIRE(
      memory.segments()
          .front()
          .sub_blocks_free_list[VMMemorySegment::EIGHT_BYTE_BLOCK_END + 2] ==
      false);
}

SIMPLE_TEST_CASE(VMMemoryLargeAllocaInTwoSegmentTest) {
  VMMemory<VirtualMachine<AggresivPolicy>> memory(256);
  std::size_t adr = memory.allocate(256);
  for (const auto &x : memory.segments()) {
    for (std::size_t i = 0; i < VMMemorySegment::SIXTEEN_BYTE_BLOCK_END; ++i) {
      REQUIRE(x.sub_blocks_free_list[i] == true);
    }
  }
  memory.deallocate(adr);
  for (auto &x : memory.segments()) {
    REQUIRE(x.sub_blocks_free_list.none());
  }
}

SIMPLE_TEST_CASE(VMMemoryLargeAllocaInOneAndHalfSegmentTest) {
  VMMemory<VirtualMachine<AggresivPolicy>> memory(256);
  std::size_t adr = memory.allocate(192);
  REQUIRE(memory.segments().front().sub_blocks_free_list.all());
  auto iter = memory.segments().begin();
  iter++;
  for (std::size_t i = 0; i < VMMemorySegment::FOUR_BYTE_BLOCK_END; ++i) {
    REQUIRE((*iter).sub_blocks_free_list[i] == true);
  }
  memory.deallocate(adr);
  for (auto &x : memory.segments()) {
    REQUIRE(x.sub_blocks_free_list.none());
  }
}

SIMPLE_TEST_CASE(VMMemoryLargeAllocaInTwoAndHalfSegmentTest) {
  VMMemory<VirtualMachine<AggresivPolicy>> memory(128 * 3);
  std::size_t adr = memory.allocate(256 + 64);
  auto iter = memory.segments().begin();
  REQUIRE((*iter).sub_blocks_free_list.all());
  iter++;
  REQUIRE((*iter).sub_blocks_free_list.all());
  iter++;
  for (std::size_t i = 0; i < VMMemorySegment::FOUR_BYTE_BLOCK_END; ++i) {
    REQUIRE((*iter).sub_blocks_free_list[i] == true);
  }

  memory.deallocate(adr);
  for (auto &x : memory.segments()) {
    REQUIRE(x.sub_blocks_free_list.none());
  }
}
