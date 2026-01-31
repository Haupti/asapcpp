#include "process.hpp"
#include "util.hpp"
#include <array>
#include <cstdlib>
#include <string>

namespace {
struct auto_pipe {
  bool closed = false;
  FILE *pipe;
  auto_pipe(const char *command) {
    pipe = popen(command, "rb");
    if (!pipe) {
      panic("failed to open pipe to '" + std::string(command) + "'");
    }
  }
  ~auto_pipe() {
    if (!closed) {
      pclose(pipe);
    }
  }
  FILE *get() { return pipe; }
  int close() {
    closed = true;
    return pclose(pipe);
  }
};
} // namespace

process_result process_run(const char *command) {
  auto_pipe pipe = auto_pipe(command);
  std::array<char, 128> buffer;
  std::string stdoutput;
  while (fgets(buffer.data(), buffer.size(), pipe.get())) {
    stdoutput += buffer.data();
  }

  return process_result(stdoutput, pipe.close());
}

void process_exec(const char *command) { system(command); }
