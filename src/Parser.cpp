#include <sqltap/Parser.h>
#include <sqltap/Tokenizer.h>
#include <sqltap/Token.h>
#include <sqltap/AllFields.h>
#include <sqltap/FieldValue.h>
#include <sqltap/DependantQuery.h>
#include <memory>
#include <string>

namespace sqltap {

Parser::Parser(Tokenizer* tokenizer) : tokenizer_(tokenizer) {
}

std::unique_ptr<Query> Parser::parse() {
  return parseQuery();
}


// relation '.' function '(' ParameterList ')' '{' FieldList '}'
std::unique_ptr<Query> Parser::parseQuery() {
  std::string relation = consume(Token::Ident);
  return parseDependantQuery(relation);
}

std::unique_ptr<Query> Parser::parseDependantQuery(const std::string& relation) {
  consume(Token::Dot);
  std::string functionName = consume(Token::Ident);

  ParameterList paramList;
  if (tokenizer()->token() == Token::RndOpen) {
    nextToken();
    if (!consumeIf(Token::RndClose)) {
      parseParamList(&paramList);
      consume(Token::RndClose);
    }
  }

  FieldList fieldList;
  if (tokenizer()->token() == Token::SetOpen) {
    nextToken();
    if (!consumeIf(Token::SetClose)) {
      parseFieldList(&fieldList);
      consume(Token::SetClose);
    }
  }

  return std::unique_ptr<Query>(new Query(
      relation, functionName,
      std::move(paramList), std::move(fieldList)));
}

void Parser::parseParamList(ParameterList* list) {
  parseParam(list);
  while (consumeIf(Token::Comma))
    parseParam(list);
}

void Parser::parseParam(ParameterList* list) {
  switch (tokenizer()->token()) {
    case Token::String:
      list->push_back(std::unique_ptr<Parameter>(
            new StringParameter(tokenizer()->stringValue())));
      nextToken();
      break;

    case Token::Number:
      list->push_back(std::unique_ptr<Parameter>(
            new NumberParameter(tokenizer()->numberValue())));
      nextToken();
      break;

    default:
      throw std::runtime_error("Unexpected parameter token.");
  }
}

void Parser::parseFieldList(FieldList* list) {
  parseField(list);
  while (consumeIf(Token::Comma))
    parseField(list);
}

void Parser::parseField(FieldList* list) {
  switch (tokenizer()->token()) {
    // TODO
    // case Token::Dollar:
    //   list->push_back(std::unique_ptr<Field>(new DependantField()));
    //   nextToken();
    //   break;

    case Token::Star:
      list->push_back(std::unique_ptr<Field>(new AllFields()));
      nextToken();
      break;

    case Token::Ident: {
      std::string ident = tokenizer()->stringValue();
      nextToken();

      if (tokenizer()->token() == Token::Dot) {
        list->push_back(std::unique_ptr<Field>(new DependantQuery(
            parseDependantQuery(ident))));
      } else {
        list->push_back(std::unique_ptr<Field>(new FieldValue(ident)));
      }
      break;
    }

    default:
      throw std::runtime_error("Unexpected field token " +
                               to_string(tokenizer()->token()) + ".");
  }
}

bool Parser::nextToken() {
  //printf("-> %s\n", tokenizer_->to_s().c_str());
  return tokenizer_->next();
}

std::string Parser::consume(Token token) {
  if (token != tokenizer()->token())
    throw std::runtime_error("Unexpected token " +
        to_string(tokenizer()->token()) + ". Expected " + to_string(token) +
        ".");

  std::string value = tokenizer()->stringValue();
  nextToken();
  return value;
}

bool Parser::consumeIf(Token token) {
  if (token != tokenizer()->token())
    return false;

  nextToken();
  return true;
}

} // namespace sqltap
