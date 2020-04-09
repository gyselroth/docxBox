
#ifndef CLASS_DOCXBOX_APP_ARGUMENTS
#define CLASS_DOCXBOX_APP_ARGUMENTS

#include <cstring>
#include <string>

#include "../../helper/helper_string.h"
#include "../../helper/helper_file.h"

namespace docxbox {

class AppArguments {
 public:
  AppArguments(int argc, char **argv);

  static std::string ResolvePathFromArgument(const std::string& pwd, int argc, char **argv, int index_argument);

  static bool IsArgumentGiven(int argc, int index, const std::string& argument_description);

  bool Matches(int offset_argument, const std::string& identifier);
  bool Matches(int offset_argument, const std::string& identifier_short, const std::string& identifier_long);

 private:
  int argc;
  char **argv;
};

} // namespace docxbox

#endif // CLASS_DOCXBOX_APP_ARGUMENTS