#pragma once

#include <sqltap/Field.h>
#include <sqltap/Query.h>
#include <memory>

namespace sqltap {

class DependantQuery : public Field {
 public:
  explicit DependantQuery(std::unique_ptr<Query>&& query);

  const Query* query() const noexcept { return query_.get(); }

  void accept(QueryVisitor& v) override;
  std::string to_s() const override;

 private:
  std::unique_ptr<Query> query_;
};

} // namespace sqltap
