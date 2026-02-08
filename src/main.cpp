#include "../lib/asap/util.hpp"
#include "command.hpp"
#include <vector>

namespace {
static std::string version = "0.1.1";
}

void show_help() {
  println("Usage: asapc <command> [args...]");
  println("Commands:\n");
  println("  new <name>         - creates a new cpp project");
  println("  build              - optimized build");
  println("  run [args]         - default build and run.");
  println("  clean              - cleanup build directory");
  println("  test               - runs the tests");
  println("  include <lib-name> - include dependency via pkg-config");
  println("  check              - runs clang-tidy on all files in src");
  println("  help|-h|--help     - shows this");
  println("  --version          - shows version");
}

int main(int args, char **argv) {
  if (args < 2) {
    show_help();
    return 0;
  }
  std::string command = argv[1];
  std::vector<std::string> arguments;
  arguments.reserve(args);
  for (int i = 2; i < args; i++) {
    arguments.push_back(argv[i]);
  }
  if (args == 2 &&
      (command == "--help" || command == "-h" || command == "help")) {
    show_help();
    return 0;
  }
  if (args == 2 && command == "--version") {
    println(version);
    return 0;
  }

  if (command == "new") {
    command_new(arguments);
  } else if (command == "test") {
    command_test(arguments);
  } else if (command == "run") {
    command_run(arguments);
  } else if (command == "build") {
    command_build(arguments);
  } else if (command == "include") {
    command_include(arguments);
  } else if (command == "check") {
    command_check(arguments);
  } else if (command == "clean") {
    command_clean(arguments);
  } else {
    fail("unknown command '" + command + "'");
  }
}
