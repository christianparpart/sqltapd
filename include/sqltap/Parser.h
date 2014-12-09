#pragma once

#include <sqltap/Query.h>
#include <sqltap/Token.h>
#include <memory>
#include <string>

namespace sqltap {

class Tokenizer;
class Manifest;

class Parser {
 public:
  explicit Parser(Tokenizer* tokenizer, Manifest* manifest);

  std::unique_ptr<Query> parse();

 private:
  Tokenizer* tokenizer() { return tokenizer_; }
  bool nextToken();

  std::string consume(Token token);
  bool consumeIf(Token token);

  std::unique_ptr<Query> parseQuery();
  std::unique_ptr<Query> parseQuery(const std::string& relation,
                                    Query* dependantQuery);

  void parseParamList(ParameterList* list);
  void parseParam(ParameterList* list);
  void parseFieldList(FieldList* list, Query* query);
  void parseField(FieldList* list, Query* query);

 private:
  Tokenizer* tokenizer_;
  Manifest* manifest_;
};

} // namespace sqltap
