#ifndef _PALLADIUM_VM_TPYE_H
#define _PALLADIUM_VM_TPYE_H
#include "Util.h"
#include <memory>
#include <string>
#include <type_traits>
#include <variant>
#include <vector>
// helper type for the visitor #4
template <class... Ts> struct overloaded : Ts... {
  using Ts::operator()...;
};
// explicit deduction guide (not needed as of C++20)
template <class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

struct VMAddress {
  explicit constexpr VMAddress() : _valid(false) {}
  explicit constexpr VMAddress(std::size_t adr) : _adr(adr), _valid(true) {}
  constexpr auto get() const -> std::size_t { return _adr; }
  constexpr auto is_valid() const -> bool { return _valid; }

private:
  std::size_t _adr;
  bool _valid;
};

using VMPrimitive =
    std::variant<int, float, std::size_t, double, bool, std::string, VMAddress>;

struct VMStruct;
using VMStructPtr = std::shared_ptr<VMStruct>;
using VMStructTypes = std::variant<VMPrimitive, VMStructPtr>;

template <class T, class VARIANT> struct get_index {
  static constexpr size_t value = VARIANT{T{}}.index();
};

struct VMStruct {
  VMStruct(VMPrimitive size);
  auto max_index() const -> std::size_t;
  void add_field(const VMStructTypes &type);
  void set_field(std::size_t index, const VMStructTypes &value);
  auto get_field(std::size_t index) -> VMStructTypes &;

private:
  std::vector<VMStructTypes> _fields;
  std::size_t _currentIndex;
};

using VMType = std::variant<VMPrimitive, VMStruct, VMAddress>;

auto add(VMType &lhs, VMType &rhs) -> ResultOr<VMType>;
auto sub(VMType &lhs, VMType &rhs) -> ResultOr<VMType>;
auto mult(VMType &lhs, VMType &rhs) -> ResultOr<VMType>;
auto div(VMType &lhs, VMType &rhs) -> ResultOr<VMType>;
auto to_string(const VMType &value) -> ResultOr<std::string>;

auto operator<(const VMPrimitive &lhs, const VMPrimitive &rhs) -> bool;
auto operator>(const VMPrimitive &lhs, const VMPrimitive &rhs) -> bool;
auto operator<=(const VMPrimitive &lhs, const VMPrimitive &rhs) -> bool;
auto operator>=(const VMPrimitive &lhs, const VMPrimitive &rhs) -> bool;
auto operator!=(const VMPrimitive &lhs, const VMPrimitive &rhs) -> bool;
auto operator==(const VMPrimitive &lhs, const VMPrimitive &rhs) -> bool;

auto to_string(const VMStructTypes &type) -> ResultOr<std::string>;

auto get_data_ptr_and_size(const VMType &type)
    -> std::pair<const void *, std::size_t>;

template <class T>
auto vm_primitive_holds_alternative(const VMPrimitive &type) -> bool {
  return std::visit(
      [](const auto &lhs) -> bool {
        using L = std::remove_cvref_t<decltype(lhs)>;
        if constexpr (std::is_same_v<L, T>) {
          return true;
        }
        return false;
      },
      type);
}

template <class T> auto is_vm_type(const VMType &type) -> bool {
  return std::visit(
      [](const auto &lhs) -> bool {
        using L = std::remove_cvref_t<decltype(lhs)>;
        if constexpr (std::is_same_v<L, VMPrimitive>) {
          return vm_primitive_holds_alternative<T>(lhs);
        } else if constexpr (std::is_same_v<L, T>) {
          return true;
        }
        return false;
      },
      type);
}

template <class T> auto vm_type_get(const VMType &type) -> ResultOr<T> {
  return std::visit(
      [](const auto &value) -> ResultOr<T> {
        using ValueType = std::remove_cvref_t<decltype(value)>;

        if constexpr (std::is_same_v<ValueType, VMPrimitive>) {
          return std::visit(
              [](const auto &inner_value) -> ResultOr<T> {
                using InnerType = std::remove_cvref_t<decltype(inner_value)>;
                if constexpr (std::is_same_v<InnerType, T>) {
                  return inner_value;
                }
                return err("Type mismatch inside VMPrimitive");
              },
              value);
        } else if constexpr (std::is_same_v<ValueType, T>) {
          return value;
        }
        return err("Type mismatch in VMType");
      },
      type);
}

template <class T>
constexpr auto get_primitive_t(const VMType &type) -> ResultOr<T> {
  if (std::holds_alternative<VMPrimitive>(type)) {
    if (std::holds_alternative<T>(std::get<VMPrimitive>(type))) {
      return std::get<T>(std::get<VMPrimitive>(type));
    }
  }
  return err("Type does not hold VMPrimitive");
}

#endif
