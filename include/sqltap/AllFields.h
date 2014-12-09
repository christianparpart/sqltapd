#pragma once

#include <sqltap/Field.h>
#include <string>

namespace sqltap {

class AllFields : public Field {
 public:
  AllFields() {}

  void accept(QueryVisitor& v) override;
  std::string to_s() const override;
};

} // namespace sqltap
