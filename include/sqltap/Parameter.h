#pragma once

#include <string>
#include <vector>
#include <memory>

namespace sqltap {

enum class ParameterType {
  String,
  Number,
};

/**
 * Query Input Parameter.
 */
class Parameter {
 public:
  Parameter(ParameterType type) : type_(type) {}
  virtual ~Parameter() {}

  ParameterType type() const noexcept { return type_; }

  virtual std::string to_s() const = 0;

 private:
  ParameterType type_;
};
typedef std::vector<std::unique_ptr<Parameter>> ParameterList;

// ----------------------------------------------------------------------------

class NumberParameter : public Parameter {
 public:
  explicit NumberParameter(int64_t value)
      : Parameter(ParameterType::Number),
        value_(value) {}

  int64_t value() const noexcept { return value_; }

  std::string to_s() const override;

 private:
  int64_t value_;
};

class StringParameter : public Parameter {
 public:
  explicit StringParameter(const std::string& value)
      : Parameter(ParameterType::String),
        value_(value) {}

  std::string value() const noexcept { return value_; }

  std::string to_s() const override;

 private:
  std::string value_;
};

} // namespace sqltap
