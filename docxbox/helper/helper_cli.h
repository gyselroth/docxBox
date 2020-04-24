// Copyright (c) 2020 gyselroth GmbH

#ifndef DOCXBOX_HELPER_HELPER_CLI_H_
#define DOCXBOX_HELPER_HELPER_CLI_H_

#include <docxbox/helper/helper_numeric.h>

#include <fcntl.h>
#include <sys/sendfile.h>

#include <algorithm>
#include <codecvt>
#include <cstring>
#include <cmath>
#include <functional>
#include <locale>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace helper {
namespace Cli {

extern void Execute(const char *command);

}  // namespace Cli
}  // namespace helper

#endif  // DOCXBOX_HELPER_HELPER_CLI_H_
