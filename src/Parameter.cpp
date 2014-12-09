#include <sqltap/Parameter.h>
#include <sqltap/QueryVisitor.h>
#include <sstream>

namespace sqltap {

void NumberParameter::accept(QueryVisitor& v) {
  return v.visit(*this);
}

std::string NumberParameter::to_s() const {
  std::ostringstream os;
  os << value();
  return os.str();
}

void StringParameter::accept(QueryVisitor& v) {
  return v.visit(*this);
}

std::string StringParameter::to_s() const {
  std::ostringstream os;
  os << '"' << value() << '"';
  return os.str();
}

void DependantParameter::accept(QueryVisitor& v) {
  return v.visit(*this);
}

std::string DependantParameter::to_s() const {
  return "$";
}

} // namespace sqltap
