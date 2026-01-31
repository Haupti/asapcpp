#include "constants.hpp"
#include "util.hpp"
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

namespace {
void write_file(const std::string &filepath, const std::string &content) {
  std::ofstream f(filepath);
  if (!f.good()) {
    panic("failed to write file '" + filepath + "'");
  }
  f << content;
}

}; // namespace

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
  write_file(args[0] + "/lib/asap/stopwatch.hpp", STOPWATCH_HEADER_FILE_CONTENT);
}
