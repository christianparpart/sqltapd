#pragma once

#include <sqltap/Token.h>
#include <cstdint>
#include <string>

namespace sqltap {

class Tokenizer {
 public:
  explicit Tokenizer(const std::string& input);
  ~Tokenizer();

  void reset();

  Token token() const noexcept { return token_; }
  std::string stringValue() const { return stringValue_; }
  int64_t numberValue() const { return numberValue_; }

  bool next();

  Tokenizer& operator++() { next(); return *this; }
  Tokenizer& operator++(int) { next(); return *this; }
  operator bool() const noexcept { return offset_ != input_.size(); }
  bool operator !() const noexcept { return offset_ == input_.size(); }

  size_t offset() const noexcept { return offset_; }
  size_t line() const noexcept { return line_; }
  size_t column() const noexcept { return column_; }

  std::string to_s() const;

 private:
  bool eof() const noexcept { return offset_ >= input_.size(); }
  int currentChar() const noexcept { return !eof() ? input_[offset_] : 0; }
  void nextChar();
  void skipSpaces();
  bool parseString();

 private:
  std::string input_;
  size_t offset_;
  size_t line_;
  size_t column_;

  Token token_;
  std::string stringValue_;
  int64_t numberValue_;
};

} // namespace sqltap
