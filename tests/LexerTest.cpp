#include "Lexer.h"
#include "LexerStream.h"
#include "purge.hpp"
PURGE_MAIN

#define LEX_REQUIERE_TOKEN(TK, X)                                                                                      \
  do {                                                                                                                 \
    KIND__ = TK;                                                                                                       \
    REQUIRE(lex.next().map(X).result_or(false));                                                                       \
  } while (false);

#define LEX_REQUIERE_TOKEN_VALUE(TK, V, X)                                                                             \
  do {                                                                                                                 \
    KIND__ = TK;                                                                                                       \
    VALUE__ = V;                                                                                                       \
    REQUIRE(lex.next().map(X).result_or(false));                                                                       \
  } while (false);

TokenKind KIND__;
std::string VALUE__;
auto test_token = [](const Token& tk) -> bool { return tk.kind() == KIND__; };
auto test_tk_value = [](const Token& tk) -> bool { return tk.kind() == KIND__ && tk.value() == VALUE__; };
SIMPLE_TEST_CASE(LexerOperatorTest) {
  auto stream = std::make_shared<LexerStringStream>("+-*/===!!=");
  Lexer lex(stream);
  LEX_REQUIERE_TOKEN(TokenKind::OP_ADD, test_token);
  LEX_REQUIERE_TOKEN(TokenKind::OP_SUB, test_token);
  LEX_REQUIERE_TOKEN(TokenKind::OP_MULT, test_token);
  LEX_REQUIERE_TOKEN(TokenKind::OP_DIV, test_token);
  LEX_REQUIERE_TOKEN(TokenKind::OP_EQ, test_token);
  LEX_REQUIERE_TOKEN(TokenKind::OP_SET, test_token);
  LEX_REQUIERE_TOKEN(TokenKind::OP_NOT, test_token);
  LEX_REQUIERE_TOKEN(TokenKind::OP_NEQ, test_token);
  LEX_REQUIERE_TOKEN(TokenKind::END_OF_FILE, test_token);
}

SIMPLE_TEST_CASE(LexerEmptyTest) {
  auto stream = std::make_shared<LexerStringStream>("");
  Lexer lexer(stream);
  auto token = lexer.next();
  REQUIRE(token.ok());
  REQUIRE(token.result().kind() == TokenKind::END_OF_FILE);
}

SIMPLE_TEST_CASE(LexerOnlyWhitespace) {
  auto stream = std::make_shared<LexerStringStream>("   \t\n  ");
  Lexer lexer(stream);
  auto token = lexer.next();
  REQUIRE(token.ok());
  REQUIRE(token.result().kind() == TokenKind::END_OF_FILE);
}

SIMPLE_TEST_CASE(LexerIdentifierTest) {
  auto stream = std::make_shared<LexerStringStream>("_name12_233_abc");
  Lexer lex(stream);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::IDENTIFIER, "_name12_233_abc", test_tk_value);
  LEX_REQUIERE_TOKEN(TokenKind::END_OF_FILE, test_token);
}

SIMPLE_TEST_CASE(LexerTextTest) {
  auto stream = std::make_shared<LexerStringStream>("\"Hallo Welt\"");
  Lexer lex(stream);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::TEXT, "Hallo Welt", test_tk_value);
}

SIMPLE_TEST_CASE(LexerMaskedQuoteTextTest) {
  auto stream = std::make_shared<LexerStringStream>("\"Hallo\\\" Welt\"");
  Lexer lex(stream);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::TEXT, "Hallo\" Welt", test_tk_value);
}

SIMPLE_TEST_CASE(LexerUnterminatedString_1) {
  auto stream = std::make_shared<LexerStringStream>("\"Hallo\\\" Welt");
  Lexer lex(stream);
  REQUIRE(!lex.next().ok());
}

SIMPLE_TEST_CASE(LexerUnterminatedString_2) {
  auto stream = std::make_shared<LexerStringStream>("\"Hallo\\");
  Lexer lex(stream);
  REQUIRE(!lex.next().ok());
}

SIMPLE_TEST_CASE(LexerUnterminatedString_3) {
  auto stream = std::make_shared<LexerStringStream>("\"unterminated");
  Lexer lexer(stream);
  auto token = lexer.next();
  REQUIRE(!token.ok());
}

SIMPLE_TEST_CASE(LexerInteger_1) {
  auto stream = std::make_shared<LexerStringStream>("123456");
  Lexer lex(stream);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::INTEGER, "123456", test_tk_value);
}

SIMPLE_TEST_CASE(LexerInteger_2) {
  auto stream = std::make_shared<LexerStringStream>("0");
  Lexer lex(stream);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::INTEGER, "0", test_tk_value);
}

SIMPLE_TEST_CASE(LexerInteger_2_failure) {
  auto stream = std::make_shared<LexerStringStream>("01");
  Lexer lex(stream);
  REQUIRE(!lex.next().ok());
}

SIMPLE_TEST_CASE(LexerInteger_3) {
  auto stream = std::make_shared<LexerStringStream>("0xFF10FF");
  Lexer lex(stream);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::INTEGER, "0xFF10FF", test_tk_value);
}

SIMPLE_TEST_CASE(LexerInteger_3_failure) {
  auto stream = std::make_shared<LexerStringStream>("0xHZK");
  Lexer lex(stream);
  REQUIRE(!lex.next().ok());
}

SIMPLE_TEST_CASE(LexerInteger_4) {
  auto stream = std::make_shared<LexerStringStream>("0b010101");
  Lexer lex(stream);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::INTEGER, "0b010101", test_tk_value);
}

SIMPLE_TEST_CASE(LexerInteger_4_failure) {
  auto stream = std::make_shared<LexerStringStream>("0b23");
  Lexer lex(stream);
  REQUIRE(!lex.next().ok());
}

SIMPLE_TEST_CASE(LexerDouble_1) {
  auto stream = std::make_shared<LexerStringStream>("1.101");
  Lexer lex(stream);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::DOUBLE, "1.101", test_tk_value);
}

SIMPLE_TEST_CASE(LexerDouble_2) {
  auto stream = std::make_shared<LexerStringStream>("0.101");
  Lexer lex(stream);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::DOUBLE, "0.101", test_tk_value);
}

SIMPLE_TEST_CASE(LexerDouble_1_failure) {
  auto stream = std::make_shared<LexerStringStream>("1.101.2");
  Lexer lex(stream);
  REQUIRE(!lex.next().ok());
}

SIMPLE_TEST_CASE(LexerDouble_2_failure) {
  auto stream = std::make_shared<LexerStringStream>("0.101.0");
  Lexer lex(stream);
  REQUIRE(!lex.next().ok());
}

SIMPLE_TEST_CASE(Lexer_complex_test_1) {
  auto stream = std::make_shared<LexerStringStream>("0.101+2-\"Hallo Welt\"=X");
  Lexer lex(stream);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::DOUBLE, "0.101", test_tk_value);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::OP_ADD, "+", test_tk_value);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::INTEGER, "2", test_tk_value);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::OP_SUB, "-", test_tk_value);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::TEXT, "Hallo Welt", test_tk_value);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::OP_SET, "=", test_tk_value);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::IDENTIFIER, "X", test_tk_value);
  LEX_REQUIERE_TOKEN(TokenKind::END_OF_FILE, test_token);
}

SIMPLE_TEST_CASE(Lexer_complex_test_2) {
  auto stream = std::make_shared<LexerStringStream>("0.1 + 2 \t-\n \"Hallo Welt\"  = X");
  Lexer lex(stream);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::DOUBLE, "0.1", test_tk_value);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::OP_ADD, "+", test_tk_value);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::INTEGER, "2", test_tk_value);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::OP_SUB, "-", test_tk_value);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::TEXT, "Hallo Welt", test_tk_value);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::OP_SET, "=", test_tk_value);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::IDENTIFIER, "X", test_tk_value);
  LEX_REQUIERE_TOKEN(TokenKind::END_OF_FILE, test_token);
}

SIMPLE_TEST_CASE(Lexer_complex_test_3) {
  auto stream = std::make_shared<LexerStringStream>("fn main(i32 a, i32 b) -> void { while( a < b ) { return 0;} }");
  Lexer lex(stream);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::FN, "fn", test_tk_value);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::IDENTIFIER, "main", test_tk_value);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::CLAMP_OPEN, "(", test_tk_value);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::I32, "i32", test_tk_value);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::IDENTIFIER, "a", test_tk_value);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::COMMA, ",", test_tk_value);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::I32, "i32", test_tk_value);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::IDENTIFIER, "b", test_tk_value);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::CLAMP_CLOSE, ")", test_tk_value);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::ARROW, "->", test_tk_value);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::VOID, "void", test_tk_value);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::CURLY_OPEN, "{", test_tk_value);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::WHILE, "while", test_tk_value);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::CLAMP_OPEN, "(", test_tk_value);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::IDENTIFIER, "a", test_tk_value);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::OP_LS, "<", test_tk_value);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::IDENTIFIER, "b", test_tk_value);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::CLAMP_CLOSE, ")", test_tk_value);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::CURLY_OPEN, "{", test_tk_value);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::RETURN, "return", test_tk_value);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::INTEGER, "0", test_tk_value);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::SEMICOLON, ";", test_tk_value);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::CURLY_CLOSE, "}", test_tk_value);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::CURLY_CLOSE, "}", test_tk_value);
  LEX_REQUIERE_TOKEN(TokenKind::END_OF_FILE, test_token);
}

SIMPLE_TEST_CASE(Lexer_Looahead_test_1) {
  auto stream = std::make_shared<LexerStringStream>("fn main(i32 a, i32 b) -> void { while( a < b ) { return 0;} }");
  Lexer lex(stream);
  REQUIRE(lex.lookahead(Token(TokenKind::FN, "fn"), Token(TokenKind::IDENTIFIER, "*")));

  LEX_REQUIERE_TOKEN_VALUE(TokenKind::FN, "fn", test_tk_value);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::IDENTIFIER, "main", test_tk_value);
}

SIMPLE_TEST_CASE(Lexer_Looahead_test_2) {
  auto stream = std::make_shared<LexerStringStream>("fn main(i32 a, i32 b) -> void { while( a < b ) { return 0;} }");
  Lexer lex(stream);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::FN, "fn", test_tk_value);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::IDENTIFIER, "main", test_tk_value);

  REQUIRE(lex.lookahead(Token(TokenKind::CLAMP_OPEN, "("), Token(TokenKind::I32, "i32")));

  LEX_REQUIERE_TOKEN_VALUE(TokenKind::CLAMP_OPEN, "(", test_tk_value);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::I32, "i32", test_tk_value);
}

SIMPLE_TEST_CASE(Lexer_Looahead_test_3) {
  auto stream = std::make_shared<LexerStringStream>("fn main(i32 a, i32 b) -> void { while( a < b ) { return 0;} }");
  Lexer lex(stream);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::FN, "fn", test_tk_value);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::IDENTIFIER, "main", test_tk_value);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::CLAMP_OPEN, "(", test_tk_value);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::I32, "i32", test_tk_value);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::IDENTIFIER, "a", test_tk_value);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::COMMA, ",", test_tk_value);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::I32, "i32", test_tk_value);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::IDENTIFIER, "b", test_tk_value);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::CLAMP_CLOSE, ")", test_tk_value);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::ARROW, "->", test_tk_value);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::VOID, "void", test_tk_value);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::CURLY_OPEN, "{", test_tk_value);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::WHILE, "while", test_tk_value);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::CLAMP_OPEN, "(", test_tk_value);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::IDENTIFIER, "a", test_tk_value);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::OP_LS, "<", test_tk_value);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::IDENTIFIER, "b", test_tk_value);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::CLAMP_CLOSE, ")", test_tk_value);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::CURLY_OPEN, "{", test_tk_value);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::RETURN, "return", test_tk_value);
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::INTEGER, "0", test_tk_value);
  REQUIRE(!lex.lookahead(Token(TokenKind::CLAMP_CLOSE, ")")));
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::SEMICOLON, ";", test_tk_value);

  REQUIRE(lex.lookahead(Token(TokenKind::CURLY_CLOSE, "}"), Token(TokenKind::CURLY_CLOSE, "}"),
                        Token(TokenKind::END_OF_FILE, "EOF")));

  LEX_REQUIERE_TOKEN_VALUE(TokenKind::CURLY_CLOSE, "}", test_tk_value);
  REQUIRE(lex.lookahead(Token(TokenKind::CURLY_CLOSE, "}")));
  LEX_REQUIERE_TOKEN_VALUE(TokenKind::CURLY_CLOSE, "}", test_tk_value);
  LEX_REQUIERE_TOKEN(TokenKind::END_OF_FILE, test_token);
  REQUIRE(!lex.lookahead(Token(TokenKind::CURLY_CLOSE, "}")));
}
