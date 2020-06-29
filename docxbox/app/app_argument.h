// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_APP_APP_ARGUMENT_H_
#define DOCXBOX_APP_APP_ARGUMENT_H_

#include <docxbox/app/app_log.h>
#include <docxbox/helper/helper_string.h>
#include <docxbox/helper/helper_file.h>

#include <cstring>
#include <string>
#include <vector>

namespace docxbox {

class AppArgument {
 public:
  AppArgument(int argc, std::vector<std::string>  argv);

  static std::string ResolvePathFromArgument(
      const std::string &pwd,
      int argc,
      const std::vector<std::string> &argv,
      int index_argument,
      bool must_exist = true);

  static bool IsArgumentGiven(int argc,
                              int index,
                              const std::string &argument_description);

  static bool AreArgumentsGiven(
      int argc,
      int index_1, const std::string &arg_description_1,
      int index_2, const std::string &arg_description_2);

  static bool AreArgumentsGiven(
      int argc,
      int index_1, const std::string &arg_description_1,
      int index_2, const std::string &arg_description_2,
      int index_3, const std::string &arg_description_3);

  bool Matches(int offset_argument, const std::string &identifier);
  bool Matches(int offset_argument,
               const std::string &identifier_short,
               const std::string &identifier_long);

  static bool isArgImageFile(
      int argc, const std::vector<std::string>& argv, int index_argument);

  static bool IsKnownOption(const std::string &str);

 private:
  int argc_;
  std::vector<std::string> argv_;
};

}  // namespace docxbox

#endif  // DOCXBOX_APP_APP_ARGUMENT_H_
