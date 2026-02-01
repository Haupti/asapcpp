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
process_result process_run(const char *command);
void process_exec(const char *command);
