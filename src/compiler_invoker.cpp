#include "compiler_invoker.hpp"
#include "process.hpp"
#include "util.hpp"
#include <cassert>

std::string to_build_object_filepath(std::string filename) {
  std::string copy = filename;
  for (size_t i = 0; i < copy.size(); i++) {
    if (copy[i] == '/' || copy[i] == '.' || copy[i] == '\\') {
      copy[i] = '_';
    }
  }
  return "build/" + copy + ".o";
}

std::string compile_file_default(asap_config *conf, std::string filename,
                                 bool use_ccache, bool silent = false) {
  assert(conf);
  std::string compile_command;
  if (use_ccache) {
    compile_command = "ccache ";
  }
  std::string object_file_path = to_build_object_filepath(filename);
  compile_command += conf->compiler_name + " -c " +
                     conf->compiler_flags_default + " " + filename + " -o " +
                     object_file_path;
  if (!silent) {
    info("Compiling " + filename + "...");
  }
  process_exec(compile_command.c_str());
  return object_file_path;
}

void link_files_default(
    asap_config *conf, std::string object_files_str,
    std::string output_filename = "___use_conf_target_name___",
    bool silent = false) {
  assert(conf);
  if (output_filename == "___use_conf_target_name___") {
    output_filename = conf->target_name;
  }
  if (!silent) {
    info("Linking...");
  }
  std::string link_command = conf->compiler_name + " " +
                             conf->linker_flags_default + " " +
                             object_files_str + " " + " -o " + output_filename;
  process_exec(link_command.c_str());
}
