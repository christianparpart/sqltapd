#pragma once

#include <string>
#include <unordered_map>
#include <string>
#include <vector>
#include <memory>

namespace sqltap {

/**
 * A field value in the Resource.
 *
 * A field value directly maps to a column in the database table schema.
 *
 * @see Resource
 */
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
                   const std::string& joinField,
                   const std::string& joinFieldLocal,
                   const std::string& joinFieldRemote,
                   const std::string& joinCondition,
                   bool joinForeign)
    : resource_(resource),
      name_(name),
      outputName_(outputName),
      joinField_(joinField),
      joinFieldLocal_(joinFieldLocal),
      joinFieldRemote_(joinFieldRemote),
      joinCondition_(joinCondition),
      joinForeign_(joinForeign) {}

  Resource* resource() const noexcept { return resource_; }
  const std::string& name() const noexcept { return name_; }
  const std::string& outputName() const noexcept { return outputName_; }
  const std::string& joinField() const noexcept { return joinField_; }
  const std::string& joinFieldLocal() const noexcept { return joinFieldLocal_; }
  const std::string& joinFieldRemote() const noexcept { return joinFieldRemote_; }
  const std::string& joinCondition() const noexcept { return joinCondition_; }
  bool joinForeign() const noexcept { return joinForeign_; }

 private:
  Resource* resource_;
  std::string name_;
  std::string outputName_;
  std::string joinField_;
  std::string joinFieldLocal_;
  std::string joinFieldRemote_;
  std::string joinCondition_;
  bool joinForeign_;
};

/**
 * A Resource directly maps to a row schema in a database table.
 *
 * @see ResourceField
 * @see ResourceRelation
 */
class Resource {
 public:
  Resource(const std::string& name,
           const std::string& tableName,
           const std::string& idFieldName,
           const std::string& defaultOrder,
           const std::vector<ResourceField>& fields,
           const std::vector<ResourceRelation>& relations)
      : name_(name),
        tableName_(tableName),
        idFieldName_(idFieldName),
        defaultOrder_(defaultOrder),
        fields_(fields),
        relations_(relations) {}

  const std::string& name() const noexcept { return name_; }
  const std::string& tableName() const noexcept { return tableName_; }
  const std::string& idFieldName() const noexcept { return idFieldName_; }
  const std::string& defaultOrder() const noexcept { return defaultOrder_; }
  const std::vector<ResourceField>& fields() const noexcept { return fields_; }
  const std::vector<ResourceRelation>& relations() const noexcept { return relations_; }

  const ResourceField* field(const std::string& name) const;
  const ResourceRelation* relation(const std::string& name) const;

 private:
  std::string name_;
  std::string tableName_;
  std::string idFieldName_;
  std::string defaultOrder_; // defaults to "$idFieldName DESC"
  std::vector<ResourceField> fields_;
  std::vector<ResourceRelation> relations_;
};

/**
 * A Manifest defines a set of Resource items.
 */
class Manifest {
 private:
  // disallow copying and default initialization
  Manifest() = delete;
  Manifest(const Manifest&) = delete;
  Manifest& operator=(Manifest&) = delete;

 public:
  /**
   * Initializes the resource manifest with the given mappings.
   */
  Manifest(std::unordered_map<std::string, Resource*>&& resources,
           std::unordered_map<std::string, Resource*>&& tableToResourceMapping);

  /**
   * Frees up any potentially dynamically created resources.
   */
  ~Manifest();

  /**
   * Retrieves a list of all resources (as vector).
   */
  std::vector<Resource*> resources() const;

  /** Finds a resource by its @p name.
   *
   * @return the resource or @c nullptr if not found.
   */
  Resource* findResourceByName(const std::string& name) const;

  /**
   * Finds a resource by database table's @p name.
   *
   * @return the resource or @c nullptr if not found.
   */
  Resource* findResourceByTableName(const std::string& name) const;

  /**
   * Loads a manifest from a given XML file.
   *
   * @throw std::runtime_error in any case of runtime error
   * @throw std::system_error in any case of system error
   */
  static std::unique_ptr<Manifest> loadFromXmlFile(const std::string& document);

 private:
  std::unordered_map<std::string, Resource*> resources_;
  std::unordered_map<std::string, Resource*> tableToResourceMapping_;
};

} // namespace sqltap
