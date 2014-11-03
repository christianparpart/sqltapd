#pragma once

#include <sqltap/Parameter.h>
#include <sqltap/Field.h>
#include <string>
#include <memory>

namespace sqltap {

class Query {
 public:
  Query(const std::string& model, const std::string functionName,
        ParameterList&& params, FieldList&& fields)
      : model_(model),
        functionName_(functionName),
        params_(std::move(params)),
        fields_(std::move(fields)) {}

  const std::string& model() const noexcept { return model_; }
  const std::string& functionName() const noexcept { return functionName_; }
  const ParameterList& params() const noexcept { return params_; }
  const FieldList& fields() const noexcept { return fields_; }

  std::string to_s() const;

 private:
  std::string model_;
  std::string functionName_;
  ParameterList params_;
  FieldList fields_;
};

// resource.findOne(id){...}
// relation.findOne{...}
// relation.findAll{...}
// relation.findAll(limit){...}
// relation.findAll(limit, offset){...}
// relation.findAllWhere("condition"){...}
// relation.findAllWhere("condition", limit){...}
// relation.findAllWhere("condition", limit, offset){...}
// relation.countAll{}
// relation.countAllWhere("condition"){}

} // namespace sqltap
