
#include "helper_system.h"

namespace helper {

// Get absolute path to application executable
std::string System::GetBinaryPath(char **argv, size_t strLenExecutableName) {
  char *path_relative = argv[0];
  char absolute_path[255];
  char *ptr;

  ptr = realpath(path_relative, absolute_path);

  unsigned long len_without_binary = std::strlen(absolute_path) - strLenExecutableName;

  return std::string(ptr).substr(0, len_without_binary);
}

} // namespace helper
