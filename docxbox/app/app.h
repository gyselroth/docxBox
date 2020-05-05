// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_APP_APP_H_
#define DOCXBOX_APP_APP_H_

#include <docxbox/app/app_argument.h>
#include <docxbox/app/app_command.h>
#include <docxbox/app/app_help.h>
#include <docxbox/docx/docx_archive.h>
#include <docxbox/docx/docx_archive_list.h>
#include <docxbox/docx/docx_archive_replace.h>
#include <docxbox/config.h>

#include <string>
#include <iostream>
#include <cstring>

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
  int argc_;
  char **argv_;

  AppCommands *command_;

  AppCommands::Commands PreProcess(AppArguments *arguments,
                                   const AppCommands::Commands &command) const;

  bool ProcessList(AppCommands::Commands command);
  bool ProcessReplace(AppCommands::Commands command);
};

}  // namespace docxbox

#endif  // DOCXBOX_APP_APP_H_
