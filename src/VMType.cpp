#include "VMType.h"
#include "Util.h"
#include <cstddef>
#include <functional>
#include <type_traits>
#include <variant>

template <class L, class R>
auto create_operator_error(const std::string &str) -> Error {
  std::string e = str;
  e += " not permitted (";
  e += typeid(L).name();
  e += ",";
  e += typeid(R).name();
  e += ")";
  return err(e);
}

auto add(VMType &lhs, VMType &rhs) -> ResultOr<VMType> {
  if (std::holds_alternative<VMPrimitive>(lhs) &&
      std::holds_alternative<VMPrimitive>(rhs)) {

    return std::visit(
        [](const auto &lhv, const auto &rhv) -> ResultOr<VMType> {
          using L = std::remove_cvref_t<decltype(lhv)>;
          using R = std::remove_cvref_t<decltype(rhv)>;

          if constexpr (std::is_arithmetic_v<L> && std::is_arithmetic_v<R>) {
            using CommonType = std::common_type_t<L, R>;
            return VMType{VMPrimitive{
                static_cast<CommonType>(lhv) +
                static_cast<CommonType>(rhv)}}; // Numerische Addition
          } else if constexpr (std::is_same_v<L, std::string> &&
                               std::is_same_v<R, std::string>) {
            return VMType{VMPrimitive{lhv + rhv}}; // String-Konkatenation
          }
          return create_operator_error<L, R>("add");
        },
        std::get<VMPrimitive>(lhs), std::get<VMPrimitive>(rhs));
  }
  return std::visit(
      [](const auto &lhv, const auto &rhv) -> ResultOr<VMType> {
        using L = std::remove_cvref_t<decltype(lhv)>;
        using R = std::remove_cvref_t<decltype(rhv)>;
        return create_operator_error<L, R>("add");
      },
      lhs, rhs);
}
auto sub(VMType &lhs, VMType &rhs) -> ResultOr<VMType> {
  if (std::holds_alternative<VMPrimitive>(lhs) &&
      std::holds_alternative<VMPrimitive>(rhs)) {

    return std::visit(
        [](const auto &lhv, const auto &rhv) -> ResultOr<VMType> {
          using L = std::remove_cvref_t<decltype(lhv)>;
          using R = std::remove_cvref_t<decltype(rhv)>;

          if constexpr (std::is_arithmetic_v<L> && std::is_arithmetic_v<R>) {
            using CommonType = std::common_type_t<L, R>;
            return VMType{VMPrimitive{
                static_cast<CommonType>(lhv) +
                static_cast<CommonType>(rhv)}}; // Numerische Addition
          }
          return create_operator_error<L, R>("sub");
        },
        std::get<VMPrimitive>(lhs), std::get<VMPrimitive>(rhs));
  }
  return std::visit(
      [](const auto &lhv, const auto &rhv) -> ResultOr<VMType> {
        using L = std::remove_cvref_t<decltype(lhv)>;
        using R = std::remove_cvref_t<decltype(rhv)>;
        return create_operator_error<L, R>("sub");
      },
      lhs, rhs);
}

auto mult(VMType &lhs, VMType &rhs) -> ResultOr<VMType> {
  if (std::holds_alternative<VMPrimitive>(lhs) &&
      std::holds_alternative<VMPrimitive>(rhs)) {

    return std::visit(
        [](const auto &lhv, const auto &rhv) -> ResultOr<VMType> {
          using L = std::remove_cvref_t<decltype(lhv)>;
          using R = std::remove_cvref_t<decltype(rhv)>;

          if constexpr (std::is_arithmetic_v<L> && std::is_arithmetic_v<R>) {
            using CommonType = std::common_type_t<L, R>;
            return VMType{VMPrimitive{
                static_cast<CommonType>(lhv) *
                static_cast<CommonType>(rhv)}}; // Numerische Addition
          }
          return create_operator_error<L, R>("mult");
        },
        std::get<VMPrimitive>(lhs), std::get<VMPrimitive>(rhs));
  }
  return std::visit(
      [](const auto &lhv, const auto &rhv) -> ResultOr<VMType> {
        using L = std::remove_cvref_t<decltype(lhv)>;
        using R = std::remove_cvref_t<decltype(rhv)>;
        return create_operator_error<L, R>("mult");
      },
      lhs, rhs);
}

auto div(VMType &lhs, VMType &rhs) -> ResultOr<VMType> {
  if (std::holds_alternative<VMPrimitive>(lhs) &&
      std::holds_alternative<VMPrimitive>(rhs)) {

    return std::visit(
        [](const auto &lhv, const auto &rhv) -> ResultOr<VMType> {
          using L = std::remove_cvref_t<decltype(lhv)>;
          using R = std::remove_cvref_t<decltype(rhv)>;

          if constexpr (std::is_arithmetic_v<L> && std::is_arithmetic_v<R>) {
            using CommonType = std::common_type_t<L, R>;
            double test_v = static_cast<double>(rhv);
            if (test_v == 0) {
              return err("Division by zero");
            }

            using CommonType = std::common_type_t<L, R>;
            return VMType{VMPrimitive{
                static_cast<CommonType>(lhv) /
                static_cast<CommonType>(rhv)}}; // Numerische Addition
          }
          return create_operator_error<L, R>("div");
        },
        std::get<VMPrimitive>(lhs), std::get<VMPrimitive>(rhs));
  }
  return std::visit(
      [](const auto &lhv, const auto &rhv) -> ResultOr<VMType> {
        using L = std::remove_cvref_t<decltype(lhv)>;
        using R = std::remove_cvref_t<decltype(rhv)>;
        return create_operator_error<L, R>("div");
      },
      lhs, rhs);
}

auto to_string(VMType &value) -> ResultOr<std::string> {
  if (std::holds_alternative<VMPrimitive>(value)) {
    return std::visit(
        [](const auto &v) -> ResultOr<std::string> {
          using L = std::remove_cvref_t<decltype(v)>;
          if constexpr (std::is_same_v<L, std::string>) {
            return v;
          } else if constexpr (std::is_arithmetic_v<L>) {
            return std::to_string(v);
          }
          return err("to_string not permitted on type " +
                     std::string(typeid(L).name()));
        },
        std::get<VMPrimitive>(value));
  }
  return err("ToString only allowed on primitive");
}

template <class L, class R> struct is_vm_primitive_adr_cmp {
  static constexpr bool value =
      std::is_arithmetic_v<L> && std::is_same_v<R, VMAddress>;
};
template <class L, class R>
constexpr bool is_vm_primitive_adr_cmp_v = is_vm_primitive_adr_cmp<L, R>::value;

template <class Func>
auto vm_primitive_cmp(const auto &_lhs, const auto &_rhs, Func &&op) -> bool {
  using L = std::remove_cvref_t<decltype(_lhs)>;
  using R = std::remove_cvref_t<decltype(_rhs)>;
  if constexpr (is_comparable_v<L, R>) {
    using CommonType = std::common_type_t<L, R>;
    return op(static_cast<CommonType>(_lhs), static_cast<CommonType>(_rhs));
  }
  if constexpr (is_vm_primitive_adr_cmp_v<L, R>) {
    return op(static_cast<std::size_t>(_lhs),
              static_cast<VMAddress>(_rhs).get());
  }
  if constexpr (is_vm_primitive_adr_cmp_v<R, L>) {
    return op(static_cast<VMAddress>(_lhs).get(),
              static_cast<std::size_t>(_rhs));
  }
  if constexpr (std::is_same_v<R, std::string> &&
                std::is_same_v<L, std::string>) {
    return op(_lhs, _rhs);
  }
  return false;
}

auto operator<(const VMPrimitive &lhs, const VMPrimitive &rhs) -> bool {
  auto visitor = [&](const auto &_lhs, const auto &_rhs) -> bool {
    return vm_primitive_cmp(_lhs, _rhs, std::less<>{});
  };
  return std::visit(visitor, lhs, rhs);
}

auto operator>(const VMPrimitive &lhs, const VMPrimitive &rhs) -> bool {
  auto visitor = [&](const auto &_lhs, const auto &_rhs) -> bool {
    return vm_primitive_cmp(_lhs, _rhs, std::greater<>{});
  };
  return std::visit(visitor, lhs, rhs);
}

auto operator<=(const VMPrimitive &lhs, const VMPrimitive &rhs) -> bool {
  auto visitor = [&](const auto &_lhs, const auto &_rhs) -> bool {
    return vm_primitive_cmp(_lhs, _rhs, std::less_equal<>{});
  };
  return std::visit(visitor, lhs, rhs);
}

auto operator>=(const VMPrimitive &lhs, const VMPrimitive &rhs) -> bool {
  auto visitor = [&](const auto &_lhs, const auto &_rhs) -> bool {
    return vm_primitive_cmp(_lhs, _rhs, std::greater_equal<>{});
  };
  return std::visit(visitor, lhs, rhs);
}

auto operator!=(const VMPrimitive &lhs, const VMPrimitive &rhs) -> bool {
  auto visitor = [&](const auto &_lhs, const auto &_rhs) -> bool {
    return vm_primitive_cmp(_lhs, _rhs, std::not_equal_to<>{});
  };
  return std::visit(visitor, lhs, rhs);
}

auto operator==(const VMPrimitive &lhs, const VMPrimitive &rhs) -> bool {
  auto visitor = [&](const auto &_lhs, const auto &_rhs) -> bool {
    return vm_primitive_cmp(_lhs, _rhs, std::equal_to<>{});
  };
  return std::visit(visitor, lhs, rhs);
}
