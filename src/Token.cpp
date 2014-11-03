#include <sqltap/Token.h>
#include <string>
#include <cstdio>

namespace sqltap {

std::string to_string(Token value) {
  switch (value) {
    case Token::Nothing: return "Nothing";
    case Token::Eof: return "EOF";
    case Token::Ident: return "<identifier>";
    case Token::String: return "<string>";
    case Token::Number: return "<number>";
    case Token::Star: return "'*'";
    case Token::Dollar: return "'$'";
    case Token::Dot: return "'.'";
    case Token::Comma: return "','";
    case Token::Semicolon: return "';'";
    case Token::RndOpen: return "'('";
    case Token::RndClose: return "')'";
    case Token::SetOpen: return "'{'";
    case Token::SetClose: return "'}'";
    default: {
      char buf[32];
      std::snprintf(buf, sizeof(buf), "<INVALID:%d>", static_cast<int>(value));
      return buf;
    }
  }
}

} // namespace sqltap
