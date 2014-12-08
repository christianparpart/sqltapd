// goal: tokenize => parse => generate sql query => execute => generate json
#include <sqltap/Manifest.h>
#include <sqltap/Tokenizer.h>
#include <sqltap/Parser.h>
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
class FindAllInstr : public Instruction { };
class CountAll : public Instruction { };
// }}}
class Result { // {{{
 public:
  Result() {}
};
// }}}
class Executor { // {{{
 public:
  virtual std::unique_ptr<Result> run(Query* query) = 0;
};

class LinearExecutor : public Executor {
 public:
  std::unique_ptr<Result> run(Query* query) override;
};

class ParallelExecutor : public Executor {
 public:
  //ParallelExecutor(xzero::Executor* e);

  std::unique_ptr<Result> run(Query* query) override;
};

std::unique_ptr<Result> LinearExecutor::run(Query* query) {
  return nullptr;
}

// }}}

} // namespace sqltap

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

int main(int argc, const char* argv[]) {
  try {
    auto manifest = sqltap::Manifest::loadFromXmlFile(findUpdir("schema.xml"));

    std::string input;
    if (!isatty(STDIN_FILENO))
      input = readstring(STDIN_FILENO);
    if (input.empty() && argc == 2)
      input = argv[1];
    if (input.empty())
      input = "users.findOne(1){ref.findOne(){*}}";

    sqltap::Tokenizer tokenizer(input);
    sqltap::Parser parser(&tokenizer);

    printf("input: %s\n", input.c_str());

    auto query = parser.parse();
    printf("query: %s\n", query->to_s().c_str());

    // std::unique_ptr<sqltap::Executor> executor(new sqltap::LinearExecutor());
    // executor->run(query.get());

  } catch (const std::exception& e) {
    printf("error caught (%s). %s\n", typeid(e).name(), e.what());
  }

  return 0;
}

