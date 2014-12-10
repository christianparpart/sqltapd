#include <gtest/gtest.h>
#include <sqltap/Tokenizer.h>

using namespace sqltap;

TEST(Tokenizer, shouldRecognizeEof) {
  Tokenizer t("");
  ASSERT_TRUE(t.token() == Token::Eof);
  ASSERT_FALSE(t.next());
}

TEST(Tokenizer, shouldRecognizeIdent) {
  Tokenizer t("foo bar");
  ASSERT_TRUE(t.token() == Token::Ident);
  ASSERT_TRUE("foo" == t.stringValue());
}

TEST(Tokenizer, shouldRecognizeNumber) {
  Tokenizer t("123");
  ASSERT_TRUE(t.token() == Token::Number);
  ASSERT_TRUE(123 == t.numberValue());
}

TEST(Tokenizer, shouldRecognizeString) {
  Tokenizer t("'foo bar'");
  ASSERT_TRUE(t.token() == Token::String);
  ASSERT_TRUE("foo bar" == t.stringValue());
}

TEST(Tokenizer, shouldRecognizeStar) {
  Tokenizer t("*");
  ASSERT_TRUE(t.token() == Token::Star);
  ASSERT_TRUE("*" == t.stringValue());
}

TEST(Tokenizer, shouldRecognizeDollar) {
  Tokenizer t("$");
  ASSERT_TRUE(t.token() == Token::Dollar);
  ASSERT_TRUE("$" == t.stringValue());
}

TEST(Tokenizer, shouldRecognizeDot) {
  Tokenizer t(".");
  ASSERT_TRUE(t.token() == Token::Dot);
  ASSERT_TRUE("." == t.stringValue());
}

TEST(Tokenizer, shouldRecognizeComma) {
  Tokenizer t(",");
  ASSERT_TRUE(t.token() == Token::Comma);
  ASSERT_TRUE("," == t.stringValue());
}

TEST(Tokenizer, shouldRecognizeSemicolon) {
  Tokenizer t(";");
  ASSERT_TRUE(t.token() == Token::Semicolon);
  ASSERT_TRUE(";" == t.stringValue());
}

TEST(Tokenizer, shouldRecognizeRndOpen) {
  Tokenizer t("(");
  ASSERT_TRUE(t.token() == Token::RndOpen);
  ASSERT_TRUE("(" == t.stringValue());
}

TEST(Tokenizer, shouldRecognizeRndClose) {
  Tokenizer t(")");
  ASSERT_TRUE(t.token() == Token::RndClose);
  ASSERT_TRUE(")" == t.stringValue());
}

TEST(Tokenizer, shouldRecognizeSetOpen) {
  Tokenizer t("{");
  ASSERT_TRUE(t.token() == Token::SetOpen);
  ASSERT_TRUE("{" == t.stringValue());
}

TEST(Tokenizer, shouldRecognizeSetClose) {
  Tokenizer t("}");
  ASSERT_TRUE(t.token() == Token::SetClose);
  ASSERT_TRUE("}" == t.stringValue());
}

TEST(Tokenizer, next) {
  Tokenizer t("foo (");
  ASSERT_TRUE("foo" == t.stringValue());
  t.next();
  ASSERT_TRUE("(" == t.stringValue());
  ASSERT_FALSE(t.next());
}

TEST(Tokenizer, operatorPlusPlus) {
  Tokenizer t("foo (");
  ASSERT_TRUE("foo" == t.stringValue());
  t++;
  ASSERT_TRUE("(" == t.stringValue());
}

TEST(Tokenizer, offset) {
  Tokenizer t("foo (");
  ASSERT_TRUE(3 == t.offset());
  t.next();
  ASSERT_TRUE(5 == t.offset());
  t.next();
  ASSERT_TRUE(5 == t.offset());
}

TEST(Tokenizer, line) {
  Tokenizer t("foo \nbar");
  ASSERT_TRUE(1 == t.line());
  t.next();
  ASSERT_TRUE(2 == t.line());
  t.next();
  ASSERT_TRUE(2 == t.line());
}

TEST(Tokenizer, column) {
  Tokenizer t("foo (");
  ASSERT_TRUE(3 == t.column());
  t.next();
  ASSERT_TRUE(5 == t.column());
  t.next();
  ASSERT_TRUE(5 == t.column());
}

TEST(Tokenizer, reset) {
  Tokenizer t("foo (");
  ASSERT_TRUE("foo" == t.stringValue());
  t.next();
  ASSERT_TRUE("(" == t.stringValue());
  t.reset();
  ASSERT_TRUE("foo" == t.stringValue());
  t.next();
  ASSERT_TRUE("(" == t.stringValue());
}

TEST(Tokenizer, shouldTokenizeCorrectly) {
  Tokenizer t("user.findOne(1) { username, email }");
  ASSERT_TRUE(t.stringValue() == "user"); t++;
  ASSERT_TRUE(t.stringValue() == "."); t++;
  ASSERT_TRUE(t.stringValue() == "findOne"); t++;
  ASSERT_TRUE(t.stringValue() == "("); t++;
  ASSERT_TRUE(t.stringValue() == "1"); t++;
  ASSERT_TRUE(t.stringValue() == ")"); t++;
  ASSERT_TRUE(t.stringValue() == "{"); t++;
  ASSERT_TRUE(t.stringValue() == "username"); t++;
  ASSERT_TRUE(t.stringValue() == ","); t++;
  ASSERT_TRUE(t.stringValue() == "email"); t++;
  ASSERT_TRUE(t.stringValue() == "}"); t++;
  ASSERT_TRUE(t.token() == Token::Eof); t++;
  ASSERT_TRUE(t.token() == Token::Eof);
  ASSERT_FALSE(t);
}
