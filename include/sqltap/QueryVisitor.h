#pragma once

namespace sqltap {

class Query;
class NumberParameter;
class StringParameter;
class DependantParameter;
class AllFields;
class FieldValue;
class DependantQuery;

class QueryVisitor {
 public:
  virtual ~QueryVisitor() {}

  // input
  virtual void visit(Query& v) = 0;
  virtual void visit(NumberParameter& v) = 0;
  virtual void visit(StringParameter& v) = 0;
  virtual void visit(DependantParameter& v) = 0;

  // output
  virtual void visit(AllFields& v) = 0;
  virtual void visit(FieldValue& v) = 0;
  virtual void visit(DependantQuery& v) = 0;
};

} // namespace sqltap
