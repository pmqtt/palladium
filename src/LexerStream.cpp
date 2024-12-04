#include "LexerStream.h"
#include <optional>

LexerFileStream::LexerFileStream(const std::string &filename)
    : _file(filename) {}

auto LexerFileStream::next() -> std::optional<char> {
  if (_file.is_open()) {
    char c;
    if (_file.get(c)) {
      return c;
    }
  }
  return std::nullopt;
}

void LexerFileStream::prev() { _file.unget(); }

LexerStringStream::LexerStringStream(std::string stream)
    : _pos(0), _buffer(stream) {}

auto LexerStringStream::next() -> std::optional<char> {
  if (_pos >= _buffer.size()) {
    _pos++;
    return std::nullopt;
  }
  char c = _buffer[_pos];
  _pos = _pos + 1;
  return c;
}

void LexerStringStream::prev() {
  if (_pos == 0) {
    return;
  }
  _pos--;
}
