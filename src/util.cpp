#include "util.hpp"
#include <filesystem>
#include <fstream>
void write_file(const std::string &filepath, const std::string &content) {
  std::ofstream f(filepath);
  if (!f.good()) {
    panic("failed to write file '" + filepath + "'");
  }
  f << content;
}
std::string read_file(const std::string &filepath) {
  std::ifstream f(filepath);
  if (!f.good()) {
    panic("failed to read file '" + filepath + "'");
  }
  uint64_t size = std::filesystem::file_size(filepath);
  std::string buffer(size, '\0');
  f.read(buffer.data(), size);
  return buffer;
}
