#include "config.hpp"
#include "../lib/asap/util.hpp"
#include <filesystem>
#include <vector>

using namespace std;

namespace {
vector<string> asap_conf_lib_compiler_flags(asap_conf *conf) {
  vector<string> lib_compiler_flags;
  lib_compiler_flags.reserve(conf->_lib_compiler_flags.size());
  for (auto [_, value] : conf->_lib_compiler_flags) {
    lib_compiler_flags.push_back(value);
  }
  return lib_compiler_flags;
}

vector<string> asap_conf_lib_linker_flags(asap_conf *conf) {
  vector<string> lib_linker_flags;
  lib_linker_flags.reserve(conf->_lib_linker_flags.size());
  for (auto [_, value] : conf->_lib_linker_flags) {
    lib_linker_flags.push_back(value);
  }
  return lib_linker_flags;
}
} // namespace

asap_conf asap_conf_load() {
  string content = read_file(".asap");

  asap_conf config = asap_conf();

  size_t start = 0;
  for (size_t i = 0; i < content.size(); i++) {
    if (content[i] == '\n' || i + 1 == content.size()) {
      size_t eq_pos = content.find("=", start);
      if (eq_pos == string::npos) {
        fail(".asap parsing failed at '" + content.substr(start, i - start) +
             "'");
      }
      string key = content.substr(start, eq_pos - start);
      string value = content.substr(eq_pos + 1, i - eq_pos - 1);
      start = i + 1;

      if (key == "compiler.name") {
        config.compiler_name = value;
      } else if (key == "compiler.flags.default") {
        config._compiler_flags_default = value;
      } else if (key == "compiler.flags.production") {
        config._compiler_flags_production = value;
      } else if (key == "linker.flags.default") {
        config._linker_flags_default = value;
      } else if (key == "linker.flags.production") {
        config._linker_flags_production = value;
      } else if (key == "target.name") {
        config.target_name = value;
      } else if (key == "tidy.checks") {
        config.tidy_checks = value;
      } else if (ends_with(key, ".linker.flags")) {
        config._lib_linker_flags[key] = value;
      } else if (ends_with(key, ".compiler.flags")) {
        config._lib_compiler_flags[key] = value;
      } else {
        fail("unknown configuration key '" + key + "'");
      }
    }
  }
  return config;
}

void asap_conf_write(asap_conf *conf, filesystem::path path) {
  std::string content = "target.name=" + conf->target_name +
                        "\n"
                        "compiler.name=" +
                        conf->compiler_name +
                        "\n"
                        "compiler.flags.default=" +
                        conf->_compiler_flags_default +
                        "\n"
                        "compiler.flags.production=" +
                        conf->_compiler_flags_production +
                        "\n"
                        "linker.flags.default=" +
                        conf->_linker_flags_default +
                        "\n"
                        "linker.flags.production=" +
                        conf->_linker_flags_production +
                        "\n"
                        "tidy.checks=" +
                        conf->tidy_checks + "\n";

  for (auto [key, value] : conf->_lib_compiler_flags) {
    content += key + "=" + value;
  }
  for (auto [key, value] : conf->_lib_linker_flags) {
    content += key + "=" + value;
  }
  write_file(path, content);
}

std::string asap_conf_compiler_flags_default(asap_conf *conf) {
  if (conf->_lib_compiler_flags.size() == 0) {
    return conf->_compiler_flags_default;
  }
  return conf->_compiler_flags_default + " " +
         join(asap_conf_lib_compiler_flags(conf), " ");
}

std::string asap_conf_compiler_flags_production(asap_conf *conf) {
  if (conf->_lib_compiler_flags.size() == 0) {
    return conf->_compiler_flags_production;
  }
  return conf->_compiler_flags_production + " " +
         join(asap_conf_lib_compiler_flags(conf), " ");
}

std::string asap_conf_linker_flags_default(asap_conf *conf) {
  if (conf->_lib_linker_flags.size() == 0) {
    return conf->_linker_flags_default;
  }
  return conf->_linker_flags_default + " " +
         join(asap_conf_lib_linker_flags(conf), " ");
}

std::string asap_conf_linker_flags_production(asap_conf *conf) {
  if (conf->_lib_linker_flags.size() == 0) {
    return conf->_linker_flags_production;
  }
  return conf->_linker_flags_production + " " +
         join(asap_conf_lib_linker_flags(conf), " ");
}
