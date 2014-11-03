#include <sqltap/Tokenizer.h>
#include <stdexcept>
#include <cctype>
#include <cstdio>

namespace sqltap {

Tokenizer::Tokenizer(const std::string& input)
    : input_(input),
      offset_(0),
      line_(1),
      column_(0),
      token_(Token::Nothing),
      stringValue_(),
      numberValue_() {
  next();
}

Tokenizer::~Tokenizer() {
}

void Tokenizer::reset() {
  offset_ = 0;
  line_ = 1;
  column_ = 0;
  token_ = Token::Nothing;
  stringValue_.clear();
  numberValue_ = 0;

  next();
}

bool Tokenizer::next() {
  skipSpaces();

  if (eof()) {
    token_ = Token::Eof;
    return false;
  }

  stringValue_.clear();
  stringValue_ += static_cast<char>(currentChar());

  switch (currentChar()) {
    case '*':
      token_ = Token::Star;
      nextChar();
      return true;
    case '"':
    case '\'':
      return parseString();
    case '.':
      token_ = Token::Dot;
      nextChar();
      return true;
    case ',':
      token_ = Token::Comma;
      nextChar();
      return true;
    case ';':
      token_ = Token::Semicolon;
      nextChar();
      return true;
    case '(':
      token_ = Token::RndOpen;
      nextChar();
      return true;
    case ')':
      token_ = Token::RndClose;
      nextChar();
      return true;
    case '{':
      token_ = Token::SetOpen;
      nextChar();
      return true;
    case '}':
      token_ = Token::SetClose;
      nextChar();
      return true;
    default:
      break;
  }

  if (std::isdigit(currentChar())) {
    token_ = Token::Number;
    numberValue_ = currentChar() - '0';
    nextChar();

    while (!eof() && (std::isdigit(currentChar()) || currentChar() == '_')) {
      stringValue_ += static_cast<char>(currentChar());
      numberValue_ *= 10;
      numberValue_ += currentChar() - '0';
      nextChar();
    }

    return true;
  }

  if (std::isalpha(currentChar())) {
    token_ = Token::Ident;
    nextChar();

    while (!eof() && (std::isalnum(currentChar()) || currentChar() == '_')) {
      stringValue_ += static_cast<char>(currentChar());
      nextChar();
    }

    return true;
  }

  throw std::runtime_error("Lexical error at " + to_s());
}

void Tokenizer::nextChar() {
  if (!eof()) {
    if (currentChar() == '\n') {
      line_++;
      column_ = 1;
    } else {
      column_++;
    }

    offset_++;
  }
}

void Tokenizer::skipSpaces() {
  again:
  while (!eof()) {
    switch (currentChar()) {
      case ' ':
      case '\t':
      case '\r':
      case '\n':
        nextChar();
        break;
      case '#':
        while (!eof() && currentChar() != '\n')
          nextChar();
        goto again;
      default:
        return;
    }
  }
}

// "string" or 'string'
bool Tokenizer::parseString() {
  int delim = currentChar();
  nextChar();

  stringValue_.clear();

  for (;;) {
    stringValue_ += static_cast<char>(currentChar());
    nextChar();

    if (currentChar() == delim) {
      nextChar();
      break;
    }
  }

  token_ = Token::String;
  return true;
}

std::string Tokenizer::to_s() const {
  char buf[256];
  std::snprintf(buf, sizeof(buf),
                "Tokenizer(offset:%zu, pos:%li,%li, token:%s, sval:%s",
                offset(), line(), column(), to_string(token()).c_str(),
                stringValue().c_str());
  return buf;
}

} // namespace sqltap
