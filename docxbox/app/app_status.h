// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_APP_APP_ERROR_H_
#define DOCXBOX_APP_APP_ERROR_H_

#include <iostream>

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
};

}  // namespace docxbox

#endif  // DOCXBOX_APP_APP_ERROR_H_
