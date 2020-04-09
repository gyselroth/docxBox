
#include "app_argument.h"

namespace docxbox {

/**
 * Constructor
 */
AppArguments::AppArguments(int argc, char **argv) {
  this->argc = argc;
  this->argv = argv;
}

// Resolve path from given argument: keep absolute or make relative from execution path
std::string AppArguments::ResolvePathFromArgument(
    const std::string& pwd,
    int argc,
    char **argv,
    int index_argument
) {
  if (argc < index_argument) throw "Missing file argument.";

  return helper::File::ResolvePath(pwd, argv[index_argument-1]);
}

bool AppArguments::IsArgumentGiven(int argc, int index, const std::string& argument_description) {
  if (argc <= index) {
    std::cout << "Missing argument: " << argument_description << "\n";

    return false;
  }

  return true;
}

bool AppArguments::Matches(int offset_argument, const std::string& identifier) {
  return argc > offset_argument
      && 0 == strcmp(argv[offset_argument], identifier.c_str());
}

bool AppArguments::Matches(
    int offset_argument,
    const std::string& identifier_short,
    const std::string& identifier_long
) {
  return argc > offset_argument
    && (0 == strcmp(argv[offset_argument], identifier_short.c_str())
        || 0 == strcmp(argv[offset_argument], identifier_long.c_str())
    );
}

} // namespace docxbox