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
    std::unordered_map<std::string, Resource*>&& resources,
    std::unordered_map<std::string, Resource*>&& tableToResourceMapping)
    : resources_(std::move(resources)),
      tableToResourceMapping_(std::move(tableToResourceMapping_)) {
}

Manifest::~Manifest() {
}

std::vector<Resource*> Manifest::resources() const {
  std::vector<Resource*> result;

  for (const auto& r: resources_)
    result.push_back(r.second);

  return result;
}

Resource* Manifest::findResourceByName(const std::string& name) const {
  auto i = resources_.find(name);
  if (i != resources_.end())
    return i->second;

  return nullptr;
}

Resource* Manifest::findResourceByTableName(const std::string& name) const {
  auto i = tableToResourceMapping_.find(name);
  if (i != tableToResourceMapping_.end())
    return i->second;

  return nullptr;
}

static bool toBool(const std::string& value) {
  if (value == "true")
    return true;

  if (value == "false")
    return false;

  throw std::runtime_error("Bad cast from '" + value + "' to boolean.");
}

/**
 * helper to quickly retrieve a optional XML element property.
 */
static std::string getProperty(xmlNodePtr n,
                               const std::string& name,
                               const std::string& defaultValue) {
  std::string result =
      (const char*) xmlGetProp(n, BAD_CAST name.c_str())
      ?: defaultValue;

  return result;
}

/**
 * helper to quickly retrieve a required XML element property.
 */
static std::string getProperty(xmlNodePtr n, const std::string& name) {
  std::string result = (const char*) xmlGetProp(n, BAD_CAST name.c_str()) ?: "";

  if (result.empty())
    throw std::runtime_error("Required XML property '" + name + "' not found.");

  return result;
}

// {{{ loadFromXmlFile() impl
std::unique_ptr<Manifest> Manifest::loadFromXmlFile(
    const std::string& document) {

  std::unordered_map<std::string, Resource*> resources;
  std::unordered_map<std::string, Resource*> tableToResourceMapping;

  xmlDocPtr doc = xmlReadFile(document.c_str(), nullptr, 0);
  if (doc == nullptr)
    throw std::runtime_error("Could not load schema XML file");

  xmlNodePtr root = xmlDocGetRootElement(doc);

  for (xmlNodePtr n = root->children; n != nullptr; n = n->next) {
    if (n->type != XML_ELEMENT_NODE)
      continue;

    if (xmlStrcmp(n->name, BAD_CAST "resource") == 0) {
      std::string name = getProperty(n, "name");
      std::string tableName = getProperty(n, "table_name");
      std::string idField = getProperty(n, "id_field", "id");
      std::string defaultOrder = getProperty(n, "default_order", idField + " DESC");

      std::vector<ResourceRelation> relations;
      std::vector<ResourceField> fields;

      for (xmlNodePtr cn = n->children; cn; cn = cn->next) {
        if (xmlStrcmp(cn->name, BAD_CAST "field") == 0) {
          std::string fieldName = getProperty(cn, "name");
          std::string fieldType = getProperty(cn, "type", ""); // XXX unused
          fields.emplace_back(fieldName, fieldType);
        }
        else if (xmlStrcmp(cn->name, BAD_CAST "relation") == 0) {
          std::string relationName = getProperty(cn, "name");
          std::string resourceName = getProperty(cn, "resource");
          printf("\tresource[%s].relation[%s]\n", name.c_str(), relationName.c_str());
          std::string outputName = getProperty(cn, "output_name", "");
          std::string joinField = getProperty(cn, "join_field", "");
          std::string joinFieldLocal = getProperty(cn, "join_field_local", "");
          std::string joinFieldRemote = getProperty(cn, "join_field_remote", "");
          std::string joinCondition = getProperty(cn, "join_cond", "");
          bool joinForeign = toBool(getProperty(cn, "join_foreign", "true"));

          Resource* resource = nullptr; // TODO: maybe not filled in yet
          // XXX maybe add relations to each resource in a 2nd stage.

          relations.emplace_back(
              resource, relationName, outputName, joinField,
              joinFieldLocal, joinFieldRemote, joinCondition, joinForeign);
        }
      }

      printf("resource[%s] tableName:%s\n", name.c_str(), tableName.c_str());

      resources[name] = new Resource(
          name, tableName, idField, defaultOrder, fields, relations);
    }
    else if (xmlStrcmp(n->name, BAD_CAST "ctree") == 0) {
      // TODO: ctree support we can safely ignore in the first milestone.
    }
  }

  xmlFreeDoc(doc);
  xmlCleanupParser();

  Manifest* m = new Manifest(
      std::move(resources),
      std::move(tableToResourceMapping));

  std::unique_ptr<Manifest> manifest(m);
  return manifest;
}
// }}}

} // namespace sqltap
