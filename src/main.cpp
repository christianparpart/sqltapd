// goal: tokenize => parse => generate sql query => execute => generate json
#include <sqltap/Manifest.h>
#include <sqltap/Tokenizer.h>
#include <sqltap/Parser.h>
#include <sqltap/QueryVisitor.h>
#include <sqltap/AllFields.h>
#include <sqltap/DependantQuery.h>
#include <sqltap/FieldValue.h>
#include <sqltap/Parameter.h>
#include <sqltap/Query.h>
#include <sys/stat.h>
#include <string>
#include <istream>
#include <stdexcept>
#include <system_error>
#include <memory>
#include <sstream>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>

namespace sqltap {

// {{{ Instruction
// ----------------------------------------------------------------------------
// instructions

class Instruction {
 public:
  virtual void execute() = 0;
  virtual std::string to_s() const = 0;
};
class FindOneInstr : public Instruction { };
class FindAllInstr : public Instruction { };
class CountAllInstr : public Instruction { };
// }}}
class Result { // {{{
 public:
  Result() {}
};
// }}}
class Executor { // {{{
 public:
  virtual std::unique_ptr<Result> run(const Query* query) = 0;
};

class LinearExecutor : public Executor {
 public:
  std::unique_ptr<Result> run(const Query* query) override;
};

class ParallelExecutor : public Executor {
 public:
  //ParallelExecutor(xzero::Executor* e);

  std::unique_ptr<Result> run(const Query* query) override;
};

std::unique_ptr<Result> LinearExecutor::run(const Query* query) {
  return nullptr;
}
// }}}
class InstructionBuilder : public QueryVisitor { // {{{
 public:
  InstructionBuilder(const Manifest* manifest)
      : manifest_(manifest) {}

  void analyze(const Query* query);

 private:
  void analyze(const Field* field);

  // QueryVisitor impl
  void visit(Query& v) override;
  void visit(NumberParameter& v) override;
  void visit(StringParameter& v) override;
  void visit(DependantParameter& v) override;
  void visit(AllFields& v) override;
  void visit(FieldValue& v) override;
  void visit(DependantQuery& v) override;

 private:
  const Manifest* manifest_;
  const Resource* resource_;
};

void InstructionBuilder::analyze(const Query* query) {
  const_cast<Query*>(query)->accept(*this);
}

void InstructionBuilder::analyze(const Field* field) {
  const_cast<Field*>(field)->accept(*this);
}

void InstructionBuilder::visit(Query& query) {
  printf("analyze query: %s\n", query.to_s().c_str());

  resource_ = manifest_->findResourceByName(query.model());
  if (!resource_)
    throw std::runtime_error("No resource found with name '" + query.model() + "'.");

  query.functionName();

  for (auto& param: query.params()) {
    param->accept(*this);
  }

  for (auto& field: query.fields()) {
    analyze(field.get());
  }
}

void InstructionBuilder::visit(NumberParameter& param) {
  printf("analyze param<int>: %s\n", param.to_s().c_str());
}

void InstructionBuilder::visit(StringParameter& param) {
  printf("analyze param<str>: %s\n", param.to_s().c_str());
}

void InstructionBuilder::visit(DependantParameter& param) {
  printf("analyze param<dep>: %s\n", param.to_s().c_str());
}

void InstructionBuilder::visit(AllFields& field) {
  printf("analyze field<any>: %s\n", field.to_s().c_str());
}

void InstructionBuilder::visit(FieldValue& field) {
  printf("analyze field<%s>: %s\n",
         resource_->field(field.name())->type().c_str(),
         field.to_s().c_str());
}

void InstructionBuilder::visit(DependantQuery& field) {
  printf("analyze field<qry>: %s\n", field.to_s().c_str());
  analyze(field.query());
}

// ----------------------------------------------------------------------------
void analyze(const Query* query, const sqltap::Manifest* manifest) {
  InstructionBuilder analyzer(manifest);
  analyzer.analyze(query);
}
// }}}

} // namespace sqltap

// {{{ helper
static std::string readstring(int fd) {
  std::ostringstream sstr;
  for (;;) {
    char buf[256];
    ssize_t n = read(fd, buf, sizeof(buf));
    if (n <= 0)
      break;
    sstr << std::string(buf, n);
  }
  return sstr.str();
}

/**
 * Retrieves the directory depth as a cardinal number.
 */
static size_t getDirectoryDepth() {
  size_t count = 0;
  char path[PATH_MAX + 1];

  if (getcwd(path, sizeof(path)) == nullptr)
    throw std::system_error(errno, std::system_category());

  for (const char* rp = path; *rp != '\0'; ++rp)
    if (*rp == '/')
      ++count;

  return count;
}

/**
 * traverse updir until we found a file with given name or the root dir.
 */
static std::string findUpdir(const std::string& fileName) {
  size_t curDepth = 1;
  size_t dirDepth = getDirectoryDepth();
  std::string path = fileName;
  struct stat st;

  std::string rootpath = "/" + fileName;

  while (stat(path.c_str(), &st) != 0) {
    path = "../" + path;

    curDepth++;
    if (curDepth > dirDepth) {
      return fileName;
    }
  }

  return path;
}

static std::string getInput(int argc, const char* argv[]) {
  std::string input;

  if (!isatty(STDIN_FILENO))
    input = readstring(STDIN_FILENO);

  if (input.empty() && argc == 2)
    input = argv[1];

  if (input.empty())
    input = "user.findOne(1){firstname, email, addresses.findOne{*}}";

  return input;
}
// }}}

int main(int argc, const char* argv[]) {
  try {
    auto manifest = sqltap::Manifest::loadFromXmlFile(findUpdir("schema.xml"));

    std::string input = getInput(argc, argv);
    sqltap::Tokenizer tokenizer(input);
    sqltap::Parser parser(&tokenizer, manifest.get());

    printf("input: %s\n\n", input.c_str());

    auto query = parser.parse();
    printf("query: %s\n\n", query->to_s().c_str());

    //sqltap::analyze(query.get(), manifest.get());

    // std::unique_ptr<sqltap::Executor> executor(new sqltap::LinearExecutor());
    // executor->run(query.get());

  } catch (const std::exception& e) {
    printf("error caught (%s). %s\n", typeid(e).name(), e.what());
  }

  return 0;
}

