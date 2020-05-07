// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

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

bool AppArguments::AreArgumentsGiven(
    int argc,
    int index_1, const std::string &arg_description_1,
    int index_2, const std::string &arg_description_2,
    bool do_throw) {
  return !(!IsArgumentGiven(argc, index_1, arg_description_1, do_throw)
      || !IsArgumentGiven(argc, index_2, arg_description_2, do_throw));
}

bool AppArguments::AreArgumentsGiven(
    int argc,
    int index_1, const std::string &arg_description_1,
    int index_2, const std::string &arg_description_2,
    int index_3, const std::string &arg_description_3,
    bool do_throw) {
  return !(!IsArgumentGiven(argc, index_1, arg_description_1, do_throw)
      || !IsArgumentGiven(argc, index_2, arg_description_2, do_throw)
      || !IsArgumentGiven(argc, index_3, arg_description_3, do_throw));
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
