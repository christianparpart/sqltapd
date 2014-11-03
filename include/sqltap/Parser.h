#pragma once

#include <sqltap/Query.h>
#include <sqltap/Token.h>
#include <memory>
#include <string>

namespace sqltap {

class Tokenizer;

class Parser {
 public:
  explicit Parser(Tokenizer* tokenizer);

  std::unique_ptr<Query> parse();

 private:
  Tokenizer* tokenizer() { return tokenizer_; }
  bool nextToken();

  std::string consume(Token token);
  bool consumeIf(Token token);
  std::unique_ptr<Query> parseQuery();
  void parseParamList(ParameterList* list);
  void parseParam(ParameterList* list);
  void parseFieldList(FieldList* list);
  void parseField(FieldList* list);
  std::unique_ptr<Query> parseDependantQuery(const std::string& relation);

 private:
  Tokenizer* tokenizer_;
};

} // namespace sqltap
