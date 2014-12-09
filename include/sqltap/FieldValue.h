#pragma once

#include <sqltap/Field.h>

namespace sqltap {

class FieldValue : public Field {
 public:
  explicit FieldValue(const std::string& columnName);

  const std::string name() const noexcept { return columnName_; }

  void accept(QueryVisitor& v) override;
  std::string to_s() const override;

 private:
  std::string columnName_;
};

} // namespace sqltap
