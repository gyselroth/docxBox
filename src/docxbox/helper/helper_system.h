
#ifndef CLASS_HELPER_SYSTEM
#define CLASS_HELPER_SYSTEM

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <string>

namespace helper {
namespace System {

// Get absolute path to application executable
extern std::string GetBinaryPath(char **argv, size_t strLenExecutableName);

} // namespace System
} // namespace helper

#endif //CLASS_HELPER_SYSTEM