// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/app/app_argument.h>

namespace docxbox {

// Constructor
AppArgument::AppArgument(int argc, char **argv) {
  argc_ = argc;
  argv_ = argv;
}

// Resolve path from given argument:
// keep absolute or make relative from execution path
std::string AppArgument::ResolvePathFromArgument(
    const std::string& pwd,
    int argc,
    char **argv,
    int index_argument,
    bool must_exist
) {
  std::string path;

  if (argc >= index_argument) {
    path = helper::File::ResolvePath(pwd, argv[index_argument-1]);
  } else {
    AppLog::NotifyError("Missing file argument");
  }

  if (!must_exist
      || path.empty()
      || helper::File::FileExists(path)) return path;

  docxbox::AppLog::NotifyError("File not found: " + path);

    return "";
}

bool AppArgument::IsArgumentGiven(int argc,
                                  int index,
                                  const std::string &argument_description) {
  return argc <= index
    ? docxbox::AppLog::NotifyError("Missing argument: " + argument_description)
    : true;
}

bool AppArgument::AreArgumentsGiven(
    int argc,
    int index_1, const std::string &arg_description_1,
    int index_2, const std::string &arg_description_2) {
  return !(!IsArgumentGiven(argc, index_1, arg_description_1)
      || !IsArgumentGiven(argc, index_2, arg_description_2));
}

bool AppArgument::AreArgumentsGiven(
    int argc,
    int index_1, const std::string &arg_description_1,
    int index_2, const std::string &arg_description_2,
    int index_3, const std::string &arg_description_3) {
  return !(!IsArgumentGiven(argc, index_1, arg_description_1)
      || !IsArgumentGiven(argc, index_2, arg_description_2)
      || !IsArgumentGiven(argc, index_3, arg_description_3));
}

bool AppArgument::Matches(int offset_argument, const std::string &identifier) {
  return argc_ > offset_argument
      && 0 == strcmp(argv_[offset_argument], identifier.c_str());
}

bool AppArgument::Matches(
    int offset_argument,
    const std::string& identifier_short,
    const std::string& identifier_long
) {
  return argc_ > offset_argument
    && (0 == strcmp(argv_[offset_argument], identifier_short.c_str())
        || 0 == strcmp(argv_[offset_argument], identifier_long.c_str()));
}

bool AppArgument::isArgImageFile(int argc, char **argv, int index_argument) {
  return argc > index_argument
      && helper::File::IsWordCompatibleImage(argv[index_argument]);
}

bool AppArgument::IsKnownOption(const std::string &str) {
  if (str[0] != '-' || str == "-") return false;

  if (str[1] != '-') {
    return
        str == "-d" || str == "-dj"
        || str == "-f" || str == "-fj"
        || str == "-i" || str == "-ij"
        || str == "-j"
        || str == "-l" || str == "-lj"
        || str == "-m" || str == "-mj"
        || str == "-s"
        || str == "-u";
  }

  return
    str == "--fields"
    || str == "--fonts"
    || str == "--indent" || str == "--images"
    || str == "--json"
    || str == "--locate"
    || str == "--media" || str == "--meta"
    || str == "--segments"
    || str == "--unified";
}

}  // namespace docxbox
