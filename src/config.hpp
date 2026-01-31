#pragma once

#include <filesystem>
#include <string>
struct asap_config {
  std::string target_name = "exe";
  std::string compiler_name = "clang++";
  std::string compiler_flags_default =
      "-Wall -Wextra -Werror -std=c++17 -fsanitize=address "
      "-fsanitize=undefined -g3";
  std::string compiler_flags_production =
      "-Wall -Wextra -Werror -std=c++17 -O2";
  std::string linker_flags_default = "-fsanitize=address -fsanitize=undefined";
  std::string linker_flags_production = "-O2";
  std::string tidy_checks =
      "performance-*,bugprone-use-after-move,bugprone-argument-comment,"
      "bugprone-dangling-handle,modernize-use-auto,modernize-use-override,"
      "modernize-use-using,modernize-make-unique,modernize-make-shared,misc-"
      "redundant-expression,readability-redundant-declaration";
};
asap_config asap_config_load();

void asap_config_write(asap_config conf, std::filesystem::path path = ".asap");
