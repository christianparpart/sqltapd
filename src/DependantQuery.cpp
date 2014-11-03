#include <sqltap/DependantQuery.h>
#include <sqltap/Query.h>

namespace sqltap {

DependantQuery::DependantQuery(std::unique_ptr<Query>&& query)
    : query_(std::move(query)) {
}

std::string DependantQuery::to_s() const {
  return query_->to_s();
}

} // namespace sqltap
