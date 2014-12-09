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

  if (dependantQuery != nullptr) {
    const ResourceRelation* relation = dependantQuery->resource()->relation(relationName);
    const Resource* resource = relation->resource();
    query->setResource(resource);
  } else {
    const Resource* resource = manifest_->findResourceByName(relationName);
    query->setResource(resource);
  }

  ParameterList paramList;
  if (consumeIf(Token::RndOpen)) {
    parseParamList(&paramList);
    consume(Token::RndClose);
  }
  query->setParams(std::move(paramList));

  FieldList fieldList;
  consume(Token::SetOpen);
  parseFieldList(&fieldList, query.get());
  consume(Token::SetClose);
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
  assert(query->resource() != nullptr);

  switch (tokenizer()->token()) {
    case Token::Star:
      nextToken();
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
        const ResourceField* field = query->resource()->field(ident);

        list->push_back(std::unique_ptr<Field>(new FieldValue(field->name())));
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
