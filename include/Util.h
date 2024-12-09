#ifndef PALLADIUM_UTIL_H_
#define PALLADIUM_UTIL_H_
#include <expected>
#include <iostream>
#include <stacktrace>
#include <string>
#include <variant>
#define UNUSED(x) (void)(x)

inline void panic_if(bool expr, int status = 1) {
  if (!expr) {
    ::exit(status);
  }
}

[[noreturn]] void panic(const std::string &message = "", int status = 1);

struct Error {
  std::string message;
  auto msg() const -> const std::string & { return message; }
};

template <class T, class E = Error> class ResultOr {
public:
  ResultOr(const T &rhs) : _result(rhs) {}
  ResultOr(const Error &rhs) : _result(rhs) {}
  ResultOr(T &&rhs) : _result(std::move(rhs)) {}
  ResultOr(Error &&rhs) : _result(std::move(rhs)) {}

  auto ok() const -> bool { return std::holds_alternative<T>(_result); }
  operator bool() const { return ok(); }

  auto result() const -> T {
    panic_if(ok());
    return std::get<T>(_result);
  }
  auto error_value() const -> const E & {
    panic_if(!ok());
    return std::get<E>(_result);
  }

  template <class Func> auto some(Func &&func) -> ResultOr<T> & {
    if (ok()) {
      std::forward<Func>(func)(std::get<T>(_result));
    }
    return *this;
  }

  template <class Func> auto error(Func &&func) -> ResultOr<T> {
    if (!ok()) {
      std::forward<Func>(func)(std::get<E>(_result));
    }
    return *this;
  }

  template <class Func>
  auto map(Func &&func) -> ResultOr<decltype(func(std::declval<T>()))> {
    if (ok()) {
      return std::forward<Func>(func)(std::get<T>(_result));
    }
    return std::get<E>(_result);
  }

  auto result_or(const T &rhs) -> T {
    if (ok()) {
      return std::get<T>(_result);
    }
    return rhs;
  }

private:
  std::variant<T, E> _result;
};

auto err(const std::string &msg) -> Error;

template <class T> class Switch {
public:
  Switch(T &value, const T &set_value, const T &reset_value)
      : _value(value), _reset_value(reset_value) {
    _value = set_value;
  }
  ~Switch() { _value = _reset_value; }
  Switch(const Switch &) = delete;
  Switch &operator=(const Switch &) = delete;

private:
  T &_value;
  T _reset_value;
};

#endif
