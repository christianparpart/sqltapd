#include <sqltap/Parameter.h>
#include <sstream>

namespace sqltap {

std::string NumberParameter::to_s() const {
  std::ostringstream os;
  os << value();
  return os.str();
}

std::string StringParameter::to_s() const {
  std::ostringstream os;
  os << '"' << value() << '"';
  return os.str();
}

} // namespace sqltap
