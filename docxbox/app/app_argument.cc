// Copyright (c) 2020 gyselroth GmbH

#include <docxbox/app/app_argument.h>

namespace docxbox {

// Constructor
AppArguments::AppArguments(int argc, char **argv) {
  argc_ = argc;
  argv_ = argv;
}

// Resolve path from given argument:
// keep absolute or make relative from execution path
std::string AppArguments::ResolvePathFromArgument(
    const std::string& pwd,
    int argc,
    char **argv,
    int index_argument
) {
  if (argc < index_argument) throw "Missing file argument.";

  return helper::File::ResolvePath(pwd, argv[index_argument-1]);
}

bool AppArguments::EnsureArgumentGiven(
    int argc,
    int index,
    const std::string &argument_description) {
  IsArgumentGiven(argc, index, argument_description, true);
}

bool AppArguments::IsArgumentGiven(int argc,
                                   int index,
                                   const std::string &argument_description,
                                   bool do_throw) {
  if (argc <= index) {
    if (do_throw) throw "Missing argument: " + argument_description + "\n";

    std::cerr << "Missing argument: " << argument_description << "\n";

    return false;
  }

  return true;
}

bool AppArguments::Matches(int offset_argument, const std::string &identifier) {
  return argc_ > offset_argument
      && 0 == strcmp(argv_[offset_argument], identifier.c_str());
}

bool AppArguments::Matches(
    int offset_argument,
    const std::string& identifier_short,
    const std::string& identifier_long
) {
  return argc_ > offset_argument
    && (0 == strcmp(argv_[offset_argument], identifier_short.c_str())
        || 0 == strcmp(argv_[offset_argument], identifier_long.c_str()));
}

}  // namespace docxbox
