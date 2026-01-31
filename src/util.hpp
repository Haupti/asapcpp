#pragma once

#include <iostream>
#include <stdexcept>
#define panic(msg)                                                             \
  throw std::runtime_error("ERROR in " + std::string(__FILE__) + "/" +         \
                           std::to_string(__LINE__) + ": " + msg + "\n");

namespace {
static std::string ASNI_RED = "\x1b[31m";
static std::string ASNI_YELLOW = "\x1b[93m";
static std::string ASNI_RESET = "\x1b[0m";
}; // namespace

inline void println(const std::string &msg) { std::cout << msg << "\n"; }

inline void print(const std::string &msg) { std::cout << msg; }
inline void warn(const std::string &msg) {
  println(ASNI_YELLOW + msg + ASNI_RESET);
}

inline void fail(const std::string &msg) {
  println(ASNI_RED + msg + ASNI_RESET);
  exit(1);
}

void write_file(const std::string &filepath, const std::string &content);
std::string read_file(const std::string &filepath);
