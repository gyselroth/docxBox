// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/app/app_error.h>

namespace docxbox {

bool AppError::Output(const std::string& message) {
  std::cout << "docxBox Error - " << message << "\n";

  return false;
}

}  // namespace docxbox
