#include "../lib/asap/stopwatch.hpp"
#include "compiler_invoker.hpp"
#include "config.hpp"
#include "constants.hpp"
#include "process.hpp"
#include "../lib/asap/util.hpp"
#include <filesystem>
#include <string>
#include <vector>
using namespace std;

namespace {
void cleanup_build_dir() {
  if (filesystem::exists("build")) {
    filesystem::remove_all("build");
  }
  filesystem::create_directories("build/.obj");
}

void ensure_tests_dir() {
  if (!filesystem::exists("tests")) {
    filesystem::create_directory("tests");
  }
}

bool has_ccache_and_warn() {
  process_result which_ccache = process_run("which ccache");
  if (which_ccache.status == 0) {
    return true;
  } else {
    warn("'ccache' is not installed. This is not required but recommended to "
         "increase compilation speed");
    return false;
  }
}

vector<string> find_cpp_files_in(vector<string> dir_paths, bool include_main) {
  vector<string> filepaths;
  string filename;
  for (auto dir_path : dir_paths) {
    for (auto dir_entry : filesystem::recursive_directory_iterator(dir_path)) {
      filename = dir_entry.path().filename();
      if (!include_main && filename == "main.cpp") {
        continue;
      }
      if (ends_with(filename, ".cpp")) {
        filepaths.push_back(dir_entry.path());
      }
    }
  }
  return filepaths;
}

string find_object_files_string() {
  vector<string> object_files;
  for (auto dir_entry : filesystem::recursive_directory_iterator("build")) {
    if (ends_with(dir_entry.path(), ".o")) {
      object_files.push_back(dir_entry.path());
    }
  }
  return join(object_files, " ");
}
} // namespace

void command_new(vector<string> args) {
  if (args.size() != 1) {
    fail("'new' expects exactly 1 argument");
  }
  std::filesystem::create_directory(args[0]);
  std::filesystem::create_directories(args[0] + "/build");
  std::filesystem::create_directories(args[0] + "/lib/asap");
  std::filesystem::create_directories(args[0] + "/src");
  std::filesystem::create_directories(args[0] + "/tests");
  write_file(args[0] + "/src/main.cpp", MAIN_FILE_CONTENT);
  write_file(args[0] + "/tests/test_example.cpp", TEST_EXAMPLE_FILE_CONTENT);
  write_file(args[0] + "/lib/asap/t.hpp", T_HEADER_FILE_CONTENT);
  write_file(args[0] + "/lib/asap/stopwatch.hpp",
             STOPWATCH_HEADER_FILE_CONTENT);
  asap_conf conf;
  asap_conf_write(&conf, args[0] + "/.asap");
}

void command_test(vector<string> args) {
  if (args.size() != 0) {
    fail("'test' expects no arguments");
  }
  StopWatch sw = StopWatch();
  asap_conf conf = asap_conf_load();
  cleanup_build_dir();
  ensure_tests_dir();
  bool use_ccache = has_ccache_and_warn();

  // gather src, lib, testutil and test files
  vector<string> files = find_cpp_files_in({"src", "lib"}, false);
  vector<string> test_files;
  for (auto dir_entry : filesystem::recursive_directory_iterator("tests")) {
    string filename = dir_entry.path().filename();
    if (starts_with(filename, "testutil_") && ends_with(filename, ".cpp")) {
      files.push_back(dir_entry.path());
    } else if (starts_with(filename, "test_") && ends_with(filename, ".cpp")) {
      test_files.push_back(dir_entry.path());
    }
  }

  // compile src, lib and testutil files
  for (auto file : files) {
    compile_file_default(&conf, file, use_ccache, false);
  }

  // gather src, lib and testutil object files
  string object_files_string = find_object_files_string();

  // compile, link and run test file + object files
  for (auto test : test_files) {
    string test_object_file =
        compile_file_default(&conf, test, use_ccache, true);

    link_files_default(&conf, object_files_string + " " + test_object_file,
                       "build/testexe");

    string evaluate_command = "./build/testexe";
    process_exec(evaluate_command.c_str());
  }
  info("Testrun took " + std::to_string(sw.stop_ms()) + "ms");
}

void command_run(vector<string> args) {
  asap_conf conf = asap_conf_load();
  cleanup_build_dir();
  bool use_ccache = has_ccache_and_warn();

  vector<string> src_files = find_cpp_files_in({"src", "lib"}, true);
  for (auto file : src_files) {
    compile_file_default(&conf, file, use_ccache, false);
  }

  string object_files_string = find_object_files_string();
  string output_file = "build/" + conf.target_name;
  link_files_default(&conf, object_files_string, output_file);
  string evaluate_command = "./" + output_file + " " + join(args, " ");

  if (args.size() > 0) {
    evaluate_command = "./" + output_file + " " + join(args, " ");
  } else {
    evaluate_command = "./" + output_file;
  }
  info("Running '" + evaluate_command + "'...");
  StopWatch sw = StopWatch();
  process_exec(evaluate_command.c_str());
  info("Run took " + std::to_string(sw.stop_ms()) + "ms");
}

void command_build(std::vector<std::string> args) {
  if (args.size() != 0) {
    fail("'build' expects no arguments");
  }
  StopWatch sw = StopWatch();
  asap_conf conf = asap_conf_load();
  cleanup_build_dir();
  bool use_ccache = has_ccache_and_warn();

  vector<string> src_files = find_cpp_files_in({"src", "lib"}, true);
  for (auto file : src_files) {
    compile_file_production(&conf, file, use_ccache, false);
  }

  string object_files_string = find_object_files_string();
  string output_file = "build/" + conf.target_name;
  link_files_production(&conf, object_files_string, output_file);
  info("Build took " + std::to_string(sw.stop_ms()) + "ms");
}
void command_include(std::vector<std::string> args) {
  if (args.size() != 1) {
    fail("'include' expects exactly 1 argument");
  }
  asap_conf conf = asap_conf_load();
  process_result which_pkg_config = process_run("which pkg-config");
  if (which_pkg_config.status != 0) {
    fail("'pkg-config' not found.");
  }

  string lib = args[0];
  string linker_flags_command = "pkg-config --libs " + lib;
  process_result linker_flags_result =
      process_run(linker_flags_command.c_str());
  if (linker_flags_result.status != 0) {
    fail("failed to find linker flags for '" + lib + "'");
  }

  string compiler_flags_command = "pkg-config --cflags " + lib;
  process_result compiler_flags_result =
      process_run(compiler_flags_command.c_str());
  if (compiler_flags_result.status != 0) {
    fail("failed to find compiler flags for '" + lib + "'");
  }

  conf._lib_compiler_flags[lib + ".compiler.flags"] =
      compiler_flags_result.stdoutput;
  conf._lib_linker_flags[lib + ".linker.flags"] = linker_flags_result.stdoutput;

  asap_conf_write(&conf);
}

void command_check(std::vector<std::string> args) {
  if (args.size() != 0) {
    fail("'check' expects no arguments");
  }
  asap_conf conf = asap_conf_load();
  process_result which_pkg_config = process_run("which clang-tidy");
  if (which_pkg_config.status != 0) {
    fail("'clang-tidy' not found.");
  }

  vector<string> src_files = find_cpp_files_in({"src"}, true);

  string command = "clang-tidy --checks=\"" + conf.tidy_checks + "\" " +
                   join(src_files, " ");
  process_exec(command.c_str());
}
void command_clean(vector<std::string> args) {
  if (args.size() != 0) {
    fail("'clean' expects no arguments");
  }
  cleanup_build_dir();
}
