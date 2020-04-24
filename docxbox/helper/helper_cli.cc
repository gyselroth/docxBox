// Copyright (c) 2020 gyselroth GmbH

#include <docxbox/helper/helper_cli.h>

namespace helper {

void Cli::Execute(const char *command) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
  system(command);
#pragma clang diagnostic pop
}

}  // namespace helper
