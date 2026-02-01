#include "compiler_invoker.hpp"
#include "config.hpp"
#include "constants.hpp"
#include "process.hpp"
#include "util.hpp"
#include <filesystem>
#include <string>
#include <vector>
using namespace std;

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
  asap_config conf;
  asap_config_write(conf, args[0] + "/.asap");
}

void command_test(vector<string> args) {
  if (args.size() != 0) {
    fail("'test' expects no arguments");
  }
  asap_config conf = asap_config_load();

  // check for ccache
  bool use_ccache = false;
  process_result which_ccache = process_run("which ccache");
  if (which_ccache.status == 0) {
    use_ccache = true;
  } else {
    warn("'ccache' is not installed. This is not required but recommended to "
         "increase compilation speed");
  }

  // gather src, lib and testutil files
  // gather test files
  vector<string> test_files;
  vector<string> files;
  for (auto dir_entry : filesystem::recursive_directory_iterator("src")) {
    string path = dir_entry.path();
    if (path == "main.cpp") {
      continue;
    }
    if (ends_with(path, ".cpp")) {
      files.push_back(dir_entry.path());
    }
  }
  for (auto dir_entry : filesystem::recursive_directory_iterator("lib")) {
    if (ends_with(dir_entry.path(), ".cpp")) {
      files.push_back(dir_entry.path());
    }
  }
  for (auto dir_entry : filesystem::recursive_directory_iterator("tests")) {
    if (starts_with(dir_entry.path(), "testutil_") &&
        ends_with(dir_entry.path(), ".cpp")) {
      files.push_back(dir_entry.path());
    } else if (starts_with(dir_entry.path(), "test_") &&
               ends_with(dir_entry.path(), ".cpp")) {
      test_files.push_back(dir_entry.path());
    }
  }

  // compile src, lib and testutil files
  for (auto file : files) {
    compile_file_default(&conf, file, use_ccache);
  }

  // gather src, lib and testutil object files
  vector<string> object_files;
  for (auto dir_entry : filesystem::recursive_directory_iterator("build")) {
    if (ends_with(dir_entry.path(), ".o")) {
      object_files.push_back(dir_entry.path());
    }
  }
  string object_files_string = join(object_files, " ");

  // compile, link and run test file + object files
  for (auto test : test_files) {
    string test_object_file = compile_file_default(&conf, test, use_ccache);

    link_files_default(&conf, object_files_string + " " + test_object_file,
                       "build/testexe");

    string evaluate_command = "./build/testexe";
    process_exec(evaluate_command.c_str());
  }
}
