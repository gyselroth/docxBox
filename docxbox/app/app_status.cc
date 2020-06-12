// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/app/app_status.h>

namespace docxbox {

bool AppStatus::Error(const std::string& message) {
  // TODO(kay): Collect messages instead of directly outputting them
  //messages_.push_back(message);

  std::cout << "docxBox Error - " << message << "\n";

  return false;
}

bool AppStatus::Warning(const std::string& message) {
  std::cout << "docxBox Warning - " << message << "\n";

  return true;
}

// TODO(kay): Add method for removing duplicate messages
//            and outputting to stdout or file

// TODO(kay): Call output method before exiting docxBox

}  // namespace docxbox
