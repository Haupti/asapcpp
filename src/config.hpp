#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>
struct asap_conf {
  std::string target_name = "exe";
  std::string compiler_name = "clang++";
  std::string _compiler_flags_default =
      "-Wall -Wextra -Werror -std=c++17 -fsanitize=address "
      "-fsanitize=undefined -g3";
  std::string _compiler_flags_production =
      "-Wall -Wextra -Werror -std=c++17 -O2";
  std::string _linker_flags_default = "-fsanitize=address -fsanitize=undefined";
  std::string _linker_flags_production = "-O2";
  std::string tidy_checks =
      "performance-*,bugprone-use-after-move,bugprone-argument-comment,"
      "bugprone-dangling-handle,modernize-use-auto,modernize-use-override,"
      "modernize-use-using,modernize-make-unique,modernize-make-shared,misc-"
      "redundant-expression,readability-redundant-declaration";
  std::unordered_map<std::string, std::string> _lib_compiler_flags;
  std::unordered_map<std::string, std::string> _lib_linker_flags;
};

asap_conf asap_conf_load();
void asap_conf_write(asap_conf *conf, std::filesystem::path path = ".asap");

std::string asap_conf_compiler_flags_default(asap_conf *conf);
std::string asap_conf_linker_flags_default(asap_conf *conf);
std::string asap_conf_compiler_flags_production(asap_conf *conf);
std::string asap_conf_linker_flags_production(asap_conf *conf);
