#include "Lexer.h"
#include "Util.h"
#include <iostream>

auto main(int argc, char **argv) -> int {
  UNUSED(argc);
  UNUSED(argv);

  Token tk(TokenKind::IDENTIFIER, "HALLO");

  std::cout << "hallo welt:" << TokenKind::IDENTIFIER << std::endl;
  return 0;
}
