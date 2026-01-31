#include "config.hpp"
#include "constants.hpp"
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
