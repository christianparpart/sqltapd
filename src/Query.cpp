#include <sqltap/Query.h>
#include <sqltap/Parameter.h>
#include <sqltap/Field.h>
#include <sstream>

namespace sqltap {

std::string Query::to_s() const {
  std::ostringstream sstr;

  sstr << model() << '.' << functionName();

  // input params
  if (!params_.empty()) {
    sstr << '(';
    sstr << params_[0]->to_s();
    for (size_t i = 1; i < params_.size(); ++i) {
      sstr << ", " << params_[i]->to_s();
    }
    sstr << ')';
  }

  // output fields
  if (!fields_.empty()) {
    sstr << '{';
    sstr << fields_[0]->to_s();
    for (size_t i = 1; i < fields_.size(); ++i) {
      sstr << ", " << fields_[i]->to_s();
    }
    sstr << '}';
  }

  return sstr.str();
}

} // namespace sqltap
