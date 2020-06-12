// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_APP_APP_STATUS_H_
#define DOCXBOX_APP_APP_STATUS_H_

#include <iostream>
#include <string>
#include <vector>

namespace docxbox {

class AppStatus {
 public:
  // TODO(kay): implement handling of different resolve modes (atm: output only)
  enum StatusResolveMode {
    Mode_Log,
    Mode_Output,
    Mode_OutputAndLog
  };

  static bool Error(const std::string &message);
  static bool Warning(const std::string &message);

 private:
  std::vector<std::string> messages_;
};

}  // namespace docxbox

#endif  // DOCXBOX_APP_APP_STATUS_H_
