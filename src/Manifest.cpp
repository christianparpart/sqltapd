#include <sqltap/Manifest.h>
#include <unordered_map>
#include <memory>
#include <libxml/parser.h>
#include <libxml/tree.h>

namespace sqltap {

// {{{ Resource
const ResourceField* Resource::field(const std::string& name) const {
  for (const ResourceField& field: fields_)
    if (field.name() == name)
      return &field;

  return nullptr;
}

const ResourceRelation* Resource::relation(const std::string& name) const {
  for (const ResourceRelation& relation: relations_)
    if (relation.name() == name)
      return &relation;

  return nullptr;
}
// }}}

Manifest::Manifest(
    std::unordered_map<std::string, std::unique_ptr<Resource>>&& resources,
    std::unordered_map<std::string, Resource*>&& tableToResourceMapping)
    : resources_(std::move(resources)),
      tableToResourceMapping_(std::move(tableToResourceMapping_)) {
}

std::vector<Resource*> Manifest::resources() const {
  std::vector<Resource*> result;

  for (const auto& r: resources_)
    result.push_back(r.second.get());

  return result;
}

Resource* Manifest::findResourceByName(const std::string& name) const {
  auto i = resources_.find(name);
  if (i != resources_.end())
    return i->second.get();

  return nullptr;
}

Resource* Manifest::findResourceByTableName(const std::string& name) const {
  auto i = tableToResourceMapping_.find(name);
  if (i != tableToResourceMapping_.end())
    return i->second;

  return nullptr;
}

// {{{ loadFromXmlFile() impl
std::unique_ptr<Manifest> Manifest::loadFromXmlFile(
    const std::string& document) {

  std::unordered_map<std::string, std::unique_ptr<Resource>> resources;
  std::unordered_map<std::string, Resource*> tableToResourceMapping;

  xmlDocPtr doc = xmlReadFile(document.c_str(), nullptr, 0);
  if (doc == nullptr)
    throw std::runtime_error("Could not load schema XML file");

  xmlNodePtr root = xmlDocGetRootElement(doc);

  for (xmlNodePtr n = root->children; n != nullptr; n = n->next) {
    if (n->type != XML_ELEMENT_NODE)
      continue;

    if (xmlStrcmp(n->name, BAD_CAST "resource") == 0) {
      std::string name = (const char*) xmlGetProp(n, BAD_CAST "name") ?: "";
      std::string tableName = (const char*) xmlGetProp(n, BAD_CAST "table_name") ?: "";
      std::string idField = (const char*) xmlGetProp(n, BAD_CAST "id_field") ?: "";
      std::string defaultOrder = (const char*) xmlGetProp(n, BAD_CAST "default_order") ?: "";

      if (name.empty())
        throw std::runtime_error("Required xml attribute 'name' missing.");

      if (tableName.empty()) {
        throw std::runtime_error(
            "Required xml attribute 'table_name' on resource '" + name +
            "' missing.");
      }

      if (idField.empty())
        idField = "id";

      if (defaultOrder.empty())
        defaultOrder = idField + " DESC";

      const std::vector<ResourceRelation> relations;
      const std::vector<ResourceField> fields;

      for (xmlNodePtr cn = n->children; cn; cn = cn->next) {
        if (xmlStrcmp(cn->name, BAD_CAST "field") == 0) {
          // TODO
          // ResourceField field;
          // fields.push_back(field);
        }
        else if (xmlStrcmp(cn->name, BAD_CAST "relation") == 0) {
          // TODO
        }
      }

      printf("resource[%s] tableName:%s\n", name.c_str(), tableName.c_str());

      resources[name] = std::unique_ptr<Resource>(new Resource(
          name, tableName, idField, defaultOrder, fields, relations));
    }

    else if (xmlStrcmp(n->name, BAD_CAST "ctree") == 0) {
      // TODO
    }
  }

  xmlFreeDoc(doc);
  xmlCleanupParser();

  // Manifest manifestd(std::move(resources),
  //                   std::move(tableToResourceMapping));

  std::unique_ptr<Manifest> manifest(new Manifest(
      std::move(resources),
      std::move(tableToResourceMapping)));

  return manifest;
}
// }}}

} // namespace sqltap
