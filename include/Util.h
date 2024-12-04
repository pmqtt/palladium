#ifndef PALLADIUM_UTIL_H_
#define PALLADIUM_UTIL_H_
#include <functional>
#include <string>
#include <variant>
#define UNUSED(x) (void)(x)

struct Error {
  std::string message;
  auto msg() const -> const std::string & { return message; }
};

template <class T> class ResultOr {
public:
  ResultOr(const T &rhs) : _result(rhs) {}
  ResultOr(const Error &rhs) : _result(rhs) {}
  ResultOr(T &&rhs) : _result(std::move(rhs)) {}
  ResultOr(Error &&rhs) : _result(std::move(rhs)) {}

  auto ok() const -> bool { return std::holds_alternative<T>(_result); }
  operator bool() const { return ok(); }
  auto result() const -> T { return std::get<0>(_result); }

  auto some(const std::function<void(const T &)> &func) -> ResultOr<T> & {
    if (ok()) {
      func(std::get<T>(_result));
    }
    return *this;
  }

  auto error(const std::function<void(const Error &)>) -> ResultOr<T> {
    if (!ok()) {
      func(std::get<Error>(_result));
    }
    return *this;
  }

  template <class Func>
  auto map(Func &&func) -> ResultOr<decltype(func(std::declval<T>()))> {
    if (ok()) {
      return func(std::get<T>(_result));
    }
    return std::get<Error>(_result);
  }

  auto error_value() const -> const Error & {
    if (!ok()) {
      return std::get<Error>(_result);
    }
  }

private:
  std::variant<T, Error> _result;
};

static auto err(const std::string &msg) -> Error {
  return Error{.message = msg};
}

#endif
