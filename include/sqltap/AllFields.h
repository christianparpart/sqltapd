#pragma once

#include <sqltap/Field.h>
#include <string>

namespace sqltap {

class AllFields : public Field {
 public:
  AllFields() {}

  std::string to_s() const override;
};

} // namespace sqltap
