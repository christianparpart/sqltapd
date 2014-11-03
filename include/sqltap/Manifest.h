#pragma once

#include <string>
#include <unordered_map>
#include <string>
#include <vector>
#include <memory>

namespace sqltap {

class ResourceField {
 public:
  ResourceField(const std::string& name, const std::string& type)
      : name_(name), type_(type) {}

  const std::string& name() const noexcept { return name_; }
  const std::string& type() const noexcept { return type_; }

 private:
  std::string name_;
  std::string type_;
};

class Resource;

class ResourceRelation {
 public:
  ResourceRelation(Resource* resource,
                   const std::string& name,
                   const std::string& outputName,
                   const std::string& joinFieldName,
                   const std::string& joinFieldLocal,
                   const std::string& joinFieldRemote,
                   bool joinForeign);

  Resource* resource() const noexcept { return resource_; }
  const std::string& name() const noexcept { return name_; }
  const std::string& outputName() const noexcept { return outputName_; }
  const std::string& joinFieldName() const noexcept { return joinFieldName_; }
  const std::string& joinFieldLocal() const noexcept { return joinFieldLocal_; }
  const std::string& joinFieldRemote() const noexcept { return joinFieldRemote_; }
  const std::string& joinCondition() const noexcept { return joinCondition_; }
  bool joinForeign() const noexcept { return joinForeign_; }

 private:
  Resource* resource_;
  std::string name_;
  std::string outputName_;
  std::string joinFieldName_;
  std::string joinFieldLocal_;
  std::string joinFieldRemote_;
  std::string joinCondition_;
  bool joinForeign_;
};

class Resource {
 public:
  Resource(const std::string& name,
           const std::string& tableName,
           const std::string& idFieldName,
           const std::string& defaultOrder,
           const std::vector<ResourceField>& fields,
           const std::vector<ResourceRelation>& relations);

  const std::string& name() const noexcept;
  const std::string& tableName() const noexcept;
  const std::string& idFieldName() const noexcept;

  ResourceField* field(const std::string& name) const;
  ResourceRelation* relation(const std::string& name) const;

 private:
  std::string name_;
  std::string tableName_;
  std::string idFieldName_;
  std::string defaultOrder_; // defaults to "$idFieldName DESC"
  std::vector<ResourceField> fields_;
  std::vector<ResourceRelation> relations_;
};

class Manifest {
 public:
  Manifest();

  Manifest(
      std::unordered_map<std::string, std::unique_ptr<Resource>>&& resources,
      std::unordered_map<std::string, Resource*>&& tableToResourceMapping);

  Resource* resource(const std::string& resourceName) const;
  std::vector<Resource*> resources() const;

  Resource* findResourceByName(const std::string& resourceName) const;
  Resource* findResourceByTableName(const std::string& resourceName) const;

  static std::unique_ptr<Manifest> loadXML(const std::string& document);

 private:
  std::unordered_map<std::string, std::unique_ptr<Resource>> resources_;
  std::unordered_map<std::string, Resource*> tableToResourceMapping_;
};

} // namespace sqltap
