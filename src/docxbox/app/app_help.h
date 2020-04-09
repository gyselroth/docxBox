// Copyright (c) 2020 gyselroth GmbH

#ifndef SRC_DOCXBOX_APP_APP_HELP_H_
#define SRC_DOCXBOX_APP_APP_HELP_H_

#include <string>
#include <iostream>

#include "../config.h"
#include "app.h"


namespace docxbox {

class AppHelp {
 public:
  static bool PrintVersion();

  static bool PrintHelp(bool with_title,
                        AppCommands::Commands command,
                        const std::string &command_identifier = "");
  static bool PrintOverview(bool with_title);
  static bool PrintHelpOnList();
  static bool PrintHelpOnListImages(bool with_title = true);
  static bool PrintHelpOnListFonts(bool with_title = true);
  static bool PrintHelpOnListMergeFields(bool with_title = true);
  static bool PrintHelpOnListMeta(bool with_title = true);
  static bool PrintHelpOnModifyMeta();
  static bool PrintHelpOnReplaceImage();
  static bool PrintHelpOnReplaceText();
  static bool PrintHelpOnGetPlainText();
  static bool PrintHelpOnUnzip(bool with_uz = true);
  static bool PrintHelpOnVersion();
  static bool PrintHelpOnZip();

  static void PrintUnknownArgumentMessage(const char *arg);
};

}  // namespace docxbox

#endif  // SRC_DOCXBOX_APP_APP_HELP_H_
