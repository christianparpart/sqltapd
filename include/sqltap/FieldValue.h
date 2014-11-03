#pragma once

#include <sqltap/Field.h>

namespace sqltap {

class FieldValue : public Field {
 public:
  explicit FieldValue(const std::string& columnName);

  std::string to_s() const override;

 private:
  std::string columnName_;
};

} // namespace sqltap
