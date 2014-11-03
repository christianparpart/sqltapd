#pragma once

#include <string>

namespace sqltap {

enum class Token {
  Nothing,
  Eof,

  Ident,
  String,
  Number,

  Star,       // *
  Dollar,     // $
  Dot,        // .
  Comma,      // ,
  Semicolon,  // ;
  RndOpen,    // (
  RndClose,   // )
  SetOpen,    // {
  SetClose,   // }
};

std::string to_string(Token value);

} // namespace sqltap
