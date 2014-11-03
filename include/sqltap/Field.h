#pragma once

#include <string>
#include <vector>
#include <memory>

namespace sqltap {

/**
 * Query Output Field.
 *
 * Such as SQL result columns, or dependant query results.
 */
class Field {
 public:
  virtual ~Field() {}
  virtual std::string to_s() const = 0;
};

typedef std::vector<std::unique_ptr<Field>> FieldList;

} // namespace sqltap
