// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_APP_APP_ERROR_H_
#define DOCXBOX_APP_APP_ERROR_H_

#include <iostream>

namespace docxbox {

class AppError {
 public:
  static bool Output(const std::string &message);

};

}  // namespace docxbox

#endif  // DOCXBOX_APP_APP_ERROR_H_
