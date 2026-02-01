#pragma once

#include <iostream>
#include <stdexcept>
#include <vector>

namespace {
static std::string ASNI_RED = "\x1b[31m";
static std::string ASNI_YELLOW = "\x1b[93m";
static std::string ASNI_BRIGHT_GRAY = "\x1b[37m";
static std::string ASNI_RESET = "\x1b[0m";
}; // namespace

// logging
inline void println(const std::string &msg) { std::cout << msg << "\n"; }

inline void print(const std::string &msg) { std::cout << msg; }
inline void warn(const std::string &msg) {
  println(ASNI_YELLOW + msg + ASNI_RESET);
}
inline void info(const std::string &msg) {
  println(ASNI_BRIGHT_GRAY + msg + ASNI_RESET);
}

// error and user error
inline void fail(const std::string &msg) {
  println(ASNI_RED + msg + ASNI_RESET);
  exit(1);
}
#define panic(msg)                                                             \
  throw std::runtime_error("ERROR in " + std::string(__FILE__) + "/" +         \
                           std::to_string(__LINE__) + ": " + msg + "\n");

// file utilities
void write_file(const std::string &filepath, const std::string &content);
std::string read_file(const std::string &filepath);

// string utilities
bool ends_with(const std::string &str, const std::string &suffix);
bool starts_with(const std::string &str, const std::string &prefix);
std::string join(const std::vector<std::string> & list, std::string delim);
