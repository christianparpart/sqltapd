#include <sqltap/FieldValue.h>
#include <sqltap/QueryVisitor.h>

namespace sqltap {

FieldValue::FieldValue(const std::string& columnName)
    : columnName_(columnName) {
}

void FieldValue::accept(QueryVisitor& v) {
  return v.visit(*this);
}

std::string FieldValue::to_s() const {
  return columnName_;
}

} // namespace sqltap

