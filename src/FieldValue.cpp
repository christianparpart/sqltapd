#include <sqltap/FieldValue.h>

namespace sqltap {

FieldValue::FieldValue(const std::string& columnName)
    : columnName_(columnName) {
}

std::string FieldValue::to_s() const {
  return columnName_;
}

} // namespace sqltap

