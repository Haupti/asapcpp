#pragma once

#include <string>
struct process_result {
  std::string stdoutput;
  int status;
  process_result(const std::string &stdoutput, int status) {
    this->stdoutput = stdoutput;
    this->status = status;
  }
};
process_result run(const char *command);
void exec(const char *command);
