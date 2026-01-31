#include "util.hpp"
#include "command.hpp"
#include <vector>

int main(int args, char **argv) {
  if (args < 2) {
    println("Usage: asapc <command> [args...]");
    println("Commands:\n");
    println("  new  <name>        - creates a new cpp project");
    println("  build              - optimized build");
    println("  run                - default build and run");
    println("  test               - runs the tests");
    println("  include <lib-name> - include dependency via pkg-config");
  }

  std::string command = argv[1];
  std::vector<std::string> arguments;
  arguments.reserve(args);
  for (int i = 2; i < args; i++) {
    arguments.push_back(argv[i]);
  }

  if(command == "new"){
    command_new(arguments);
  }
}
