#pragma once

#include <iostream>
#include <vector>

namespace {
static std::string ANSI_RED = "\x1b[31m";
static std::string ANSI_YELLOW = "\x1b[93m";
static std::string ANSI_BRIGHT_GRAY = "\x1b[37m";
static std::string ANSI_CR = "\r";
static std::string ANSI_CURSOR_UP = "\x1b[A";
static std::string ANSI_CLEAR_LINE = "\x1b[2K";
static std::string ANSI_RESET = "\x1b[0m";
}; // namespace

// logging
inline void println(const std::string &msg) { std::cout << msg << "\n"; }

inline void print(const std::string &msg) { std::cout << msg; }
inline void warn(const std::string &msg) {
  println(ANSI_YELLOW + msg + ANSI_RESET);
}
inline void info(const std::string &msg) {
  println(ANSI_BRIGHT_GRAY + msg + ANSI_RESET);
}

// error and user error
inline void fail [[noreturn]] (const std::string &msg) {
  println(ANSI_RED + msg + ANSI_RESET);
  exit(1);
}
#define panic(msg)                                                             \
  throw std::runtime_error("ERROR in " + std::string(__FILE__) + "/" +         \
                           std::to_string(__LINE__) + ": " + msg + "\n");

inline void info_update_line(const std::string &msg) {
  println(ANSI_CURSOR_UP + ANSI_CLEAR_LINE + ANSI_CR + ANSI_BRIGHT_GRAY + msg +
          ANSI_RESET);
}
// file utilities
void write_file(const std::string &filepath, const std::string &content);
std::string read_file(const std::string &filepath);

// string utilities
bool ends_with(const std::string &str, const std::string &suffix);
bool starts_with(const std::string &str, const std::string &prefix);
std::string join(const std::vector<std::string> &list, std::string delim);
