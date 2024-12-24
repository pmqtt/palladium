#ifndef PALLADIUM_LEXER_STREAM_H
#define PALLADIUM_LEXER_STREAM_H
#include <fstream>
#include <optional>
#include <string>

class LexerStream {
public:
  LexerStream() = default;
  virtual ~LexerStream() = default;
  virtual auto next() -> std::optional<char> = 0;
  virtual void prev() = 0;
};

class LexerFileStream : public LexerStream {
public:
  LexerFileStream(const std::string& filename);
  auto next() -> std::optional<char> override;
  void prev() override;

private:
  std::ifstream _file;
};

class LexerStringStream : public LexerStream {
public:
  LexerStringStream(std::string stream);
  auto next() -> std::optional<char> override;
  void prev() override;

private:
  std::size_t _pos;
  std::string _buffer;
};

#endif
