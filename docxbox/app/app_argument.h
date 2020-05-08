// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_APP_APP_ARGUMENT_H_
#define DOCXBOX_APP_APP_ARGUMENT_H_

#include <docxbox/helper/helper_string.h>
#include <docxbox/helper/helper_file.h>

#include <cstring>
#include <string>

namespace docxbox {

class AppArguments {
 public:
  AppArguments(int argc, char **argv);

  static std::string ResolvePathFromArgument(const std::string &pwd,
                                             int argc,
                                             char **argv,
                                             int index_argument);

  static bool IsArgumentGiven(int argc,
                              int index,
                              const std::string &argument_description,
                              bool do_throw = false);

  static bool AreArgumentsGiven(
      int argc,
      int index_1, const std::string &arg_description_1,
      int index_2, const std::string &arg_description_2,
      bool do_throw = false);

  static bool AreArgumentsGiven(
      int argc,
      int index_1, const std::string &arg_description_1,
      int index_2, const std::string &arg_description_2,
      int index_3, const std::string &arg_description_3,
      bool do_throw = false);

  bool Matches(int offset_argument, const std::string &identifier);
  bool Matches(int offset_argument,
               const std::string &identifier_short,
               const std::string &identifier_long);

 private:
  int argc_;
  char **argv_;
};

}  // namespace docxbox

#endif  // DOCXBOX_APP_APP_ARGUMENT_H_
