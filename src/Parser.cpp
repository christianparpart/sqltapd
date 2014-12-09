#include <sqltap/Parser.h>
#include <sqltap/Tokenizer.h>
#include <sqltap/Token.h>
#include <sqltap/AllFields.h>
#include <sqltap/FieldValue.h>
#include <sqltap/DependantQuery.h>
#include <sqltap/Manifest.h>
#include <cassert>
#include <memory>
#include <string>

namespace sqltap {

Parser::Parser(Tokenizer* tokenizer, Manifest* manifest)
    : tokenizer_(tokenizer),
      manifest_(manifest) {
}

std::unique_ptr<Query> Parser::parse() {
  return parseQuery();
}


// relation '.' function '(' ParameterList ')' '{' FieldList '}'
std::unique_ptr<Query> Parser::parseQuery() {
  std::string resourceName = consume(Token::Ident);

  consume(Token::Dot);

  return parseQuery(resourceName, nullptr);
}

std::unique_ptr<Query> Parser::parseQuery(const std::string& relationName,
    Query* dependantQuery) {
  std::string functionName = consume(Token::Ident);

  std::unique_ptr<Query> query(new Query(
        relationName, functionName, dependantQuery));

  ParameterList paramList;
  if (tokenizer()->token() == Token::RndOpen) {
    nextToken();
    if (!consumeIf(Token::RndClose)) {
      parseParamList(&paramList);
      consume(Token::RndClose);
    }
  }
  query->setParams(std::move(paramList));

  FieldList fieldList;
  if (tokenizer()->token() == Token::SetOpen) {
    nextToken();
    if (!consumeIf(Token::SetClose)) {
      parseFieldList(&fieldList, query.get());
      consume(Token::SetClose);
    }
  }
  query->setFields(std::move(fieldList));

  return query;
}

void Parser::parseParamList(ParameterList* list) {
  parseParam(list);
  while (consumeIf(Token::Comma))
    parseParam(list);
}

void Parser::parseParam(ParameterList* list) {
  switch (tokenizer()->token()) {
    case Token::Dollar:
      //list->emplace_back(new DependantParameter());
      nextToken();
      break;

    case Token::String:
      list->emplace_back(new StringParameter(tokenizer()->stringValue()));
      nextToken();
      break;

    case Token::Number:
      list->emplace_back(new NumberParameter(tokenizer()->numberValue()));
      nextToken();
      break;

    default:
      throw std::runtime_error("Unexpected parameter token.");
  }
}

void Parser::parseFieldList(FieldList* list, Query* query) {
  parseField(list, query);
  while (consumeIf(Token::Comma))
    parseField(list, query);
}

void Parser::parseField(FieldList* list, Query* query) {
  switch (tokenizer()->token()) {
    case Token::Star:
      nextToken();
      assert(query->resource() != nullptr);
      printf("parseField(): *: resource(%p)\n", query->resource());
      printf("parseField(): *: resource.fields.size() %zu\n", query->resource()->fields().size());
      for (const ResourceField& field: query->resource()->fields()) {
        list->emplace_back(new FieldValue(field.name()));
      }
      break;

    case Token::Ident: {
      std::string ident = tokenizer()->stringValue();
      nextToken();

      if (tokenizer()->token() == Token::Dot) {
        consume(Token::Dot);
        list->push_back(std::unique_ptr<Field>(new DependantQuery(
            parseQuery(ident, query))));
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
