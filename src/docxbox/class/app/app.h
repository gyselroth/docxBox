
#ifndef CLASS_DOCXBOX_APP
#define CLASS_DOCXBOX_APP

#include <string>
#include <iostream>
#include <cstring>

#include "app_argument.h"
#include "app_command.h"
#include "app_help.h"
#include "../docx/docx_archive.h"
#include "../../config.h"

namespace docxbox {

class App {
 public:
  // Meta constants
  static const std::string kAppName;
  static const std::string kAppExecutableName;

  // Constructor: init (resolve) command and arguments
  App(int argc, char **argv);

  // Process command + arguments
  bool Process();

 private:
  int argc;
  char **argv;

  AppCommands *command_;

  AppCommands::Command PreProcess(AppArguments *arguments, AppCommands::Command &command) const;
};

} // namespace docxbox

#endif // CLASS_DOCXBOX_APP