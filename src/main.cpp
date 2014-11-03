// goal: tokenize => parse => generate sql query => execute => generate json
#include <sqltap/Manifest.h>
#include <sqltap/Tokenizer.h>
#include <sqltap/Parser.h>
#include <string>
#include <istream>
#include <stdexcept>
#include <memory>
#include <sstream>
#include <unistd.h>

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

std::string readstring(int fd) {
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

int main(int argc, const char* argv[]) {
  try {
    auto manifest = sqltap::Manifest::loadFromXmlFile("../dwn.xml");

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

