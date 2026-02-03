#include "compiler_invoker.hpp"
#include "../lib/asap/stopwatch.hpp"
#include "process.hpp"
#include "util.hpp"
#include <cassert>

namespace {
std::string compile_file(std::string compiler_name, std::string compiler_flags,
                         std::string filename, bool use_ccache,
                         bool silent = false) {
  std::string compile_command;
  if (use_ccache) {
    compile_command = "ccache ";
  }
  std::string object_file_path = to_build_object_filepath(filename);
  compile_command += compiler_name + " -c " + compiler_flags + " " + filename +
                     " -o " + object_file_path;
  if (!silent) {
    info("Compiling " + filename + "...");
  }
  StopWatch sw = StopWatch();
  process_exec(compile_command.c_str());
  if (!silent) {
    info_update_line("Compiled " + filename + " [" +
                     std::to_string(sw.stop_ms()) + "ms]");
  }
  return object_file_path;
}

void link_files(std::string compiler_name, std::string linker_flags,
                std::string object_files_str, std::string output_filename) {
  StopWatch sw = StopWatch();
  std::string link_command = compiler_name + " " + linker_flags + " " +
                             object_files_str + " " + " -o " + output_filename;
  process_exec(link_command.c_str());
}
} // namespace

std::string to_build_object_filepath(std::string filename) {
  std::string copy = filename;
  for (size_t i = 0; i < copy.size(); i++) {
    if (copy[i] == '/' || copy[i] == '.' || copy[i] == '\\') {
      copy[i] = '_';
    }
  }
  return "build/.obj/" + copy + ".o";
}

std::string compile_file_default(asap_conf *conf, std::string filename,
                                 bool use_ccache, bool silent = false) {
  assert(conf);
  return compile_file(conf->compiler_name,
                      asap_conf_compiler_flags_default(conf), filename,
                      use_ccache, silent);
}

std::string compile_file_production(asap_conf *conf, std::string filename,
                                    bool use_ccache, bool silent = false) {
  assert(conf);
  return compile_file(conf->compiler_name,
                      asap_conf_compiler_flags_production(conf), filename,
                      use_ccache, silent);
}

void link_files_production(asap_conf *conf, std::string object_files_str,
                           std::string output_filename) {
  assert(conf);
  link_files(conf->compiler_name, asap_conf_linker_flags_production(conf),
             object_files_str, output_filename);
}

void link_files_default(asap_conf *conf, std::string object_files_str,
                        std::string output_filename) {
  assert(conf);
  link_files(conf->compiler_name, asap_conf_linker_flags_default(conf),
             object_files_str, output_filename);
}
