#pragma once

#include <sqltap/Parameter.h>
#include <sqltap/Field.h>
#include <string>
#include <memory>

namespace sqltap {

class QueryVisitor;
class Resource;

class Query {
 public:
  Query(const std::string& model, const std::string& functionName, Query* dep)
      : Query(model, functionName, dep, {}, {}, nullptr) {}

  Query(const std::string& model, const std::string functionName, Query* dep,
        ParameterList&& params, FieldList&& fields, Resource* resource)
      : model_(model),
        functionName_(functionName),
        dependantQuery_(dep),
        params_(std::move(params)),
        fields_(std::move(fields)),
        resource_(resource) {}

  const std::string& model() const noexcept { return model_; }
  const std::string& functionName() const noexcept { return functionName_; }
  Query* dependantQuery() const noexcept { return dependantQuery_; }
  const ParameterList& params() const noexcept { return params_; }
  const FieldList& fields() const noexcept { return fields_; }
  const Resource* resource() const noexcept { return resource_; }

  void setParams(ParameterList&& params) { params_ = std::move(params); }
  void setFields(FieldList&& fields) { fields_ = std::move(fields); }
  void setResource(const Resource* resource);

  std::string to_s() const;

  virtual void accept(QueryVisitor& v);

 private:
  std::string model_;
  std::string functionName_;
  Query* dependantQuery_;
  ParameterList params_;
  FieldList fields_;

  const Resource* resource_;
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
