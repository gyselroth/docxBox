// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_APP_APP_H_
#define DOCXBOX_APP_APP_H_

#include <docxbox/app/app_argument.h>
#include <docxbox/app/app_command.h>
#include <docxbox/app/app_help.h>
#include <docxbox/docx/archive/docx_archive.h>
#include <docxbox/docx/archive/docx_archive_list.h>
#include <docxbox/docx/archive/docx_archive_replace.h>
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
  App(int argc, char **argv, bool is_batch_mode = false);

  // Process command + arguments
  bool Process();

 private:
  int argc_;
  char **argv_;

  bool is_batch_mode_ = false;

  AppCommands *command_;

  AppCommands::Command PreProcess(AppArguments *arguments,
                                  const AppCommands::Command &command) const;

  bool ProcessList(AppCommands::Command command);
  bool ProcessReplace(AppCommands::Command command);
};

}  // namespace docxbox

#endif  // DOCXBOX_APP_APP_H_
