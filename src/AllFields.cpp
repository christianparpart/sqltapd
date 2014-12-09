#include <sqltap/AllFields.h>
#include <sqltap/QueryVisitor.h>

namespace sqltap {

std::string AllFields::to_s() const {
  return "*";
}

void AllFields::accept(QueryVisitor& v) {
  return v.visit(*this);
}

} // namespace sqltap

