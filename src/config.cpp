#include "config.hpp"
#include "util.hpp"
#include <filesystem>

using namespace std;
asap_config asap_config_load() {
  string content = read_file(".asap");

  asap_config config = asap_config();

  size_t start = 0;
  for (size_t i = 0; i < content.size(); i++) {
    if (content[i] == '\n' || i + 1 == content.size()) {
      size_t eq_pos = content.find("=", start);
      if (eq_pos == string::npos) {
        fail(".asap parsing failed at '" + content.substr(start, i) + "'");
      }
      string key = content.substr(start, eq_pos);
      string value = content.substr(eq_pos + 1, i);
      start = i + 1;

      if (key == "compiler.name") {
        config.compiler_name = value;
      } else if (key == "compiler.flags.default") {
        config.compiler_flags_default = value;
      } else if (key == "compiler.flags.production") {
        config.compiler_flags_production = value;
      } else if (key == "linker.flags.default") {
        config.linker_flags_default = value;
      } else if (key == "linker.flags.production") {
        config.linker_flags_production = value;
      } else if (key == "target.name") {
        config.target_name = value;
      } else if (key == "tidy.checks") {
        config.tidy_checks = value;
      } else {
        fail("unknown configuration key '" + key + "'");
      }
    }
  }
  return config;
}

void asap_config_write(asap_config conf, filesystem::path path) {
  std::string content = "target.name=" + conf.target_name +
                        "\n"
                        "compiler.name=" +
                        conf.compiler_name +
                        "\n"
                        "compiler.flags.default=" +
                        conf.compiler_flags_default +
                        "\n"
                        "compiler.flags.production=" +
                        conf.compiler_flags_production +
                        "\n"
                        "linker.flags.default=" +
                        conf.linker_flags_default +
                        "\n"
                        "linker.flags.production=" +
                        conf.linker_flags_production +
                        "\n"
                        "tidy.checks=" +
                        conf.tidy_checks + "\n";
  write_file(path, content);
}
