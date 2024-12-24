#include "Util.h"

void panic(const std::string& message, int status) {
  std::cerr << message << "\n";
  ::exit(status);
}

auto err(const std::string& msg) -> Error {
  return Error{.message = msg};
}
