// Copyright (c) 2020 gyselroth GmbH

#ifndef DOCXBOX_APP_APP_COMMAND_H_
#define DOCXBOX_APP_APP_COMMAND_H_

#include <string>
#include <utility>

namespace docxbox {

class AppCommands {
 public:
  enum Commands {
    Command_Help,
    Command_GetPlainText,
    Command_GetPlainTextSegments,
    Command_List,
    Command_ListAsJson,
    Command_ListImages,
    Command_ListImagesAsJson,
    Command_ListFonts,
    Command_ListFontsAsJson,
    Command_ListMergeFields,
    Command_ListMergeFieldsAsJson,
    Command_ListMeta,
    Command_ListMetaAsJson,
    Command_LoremIpsum,
    Command_ModifyMeta,
    Command_ReplaceImage,
    Command_ReplaceText,
    Command_Unzip,
    Command_UnzipMedia,
    Command_Version,
    Command_Zip,
    Command_Invalid
  };

  explicit AppCommands(std::string argc);

  static Commands ResolveCommandByName(const std::string &command);

  Commands GetResolved();

 private:
  // Argument value as received from CLI
  std::string argc_;

  // Argument resolved to Commands-enum
  Commands resolved_;

  // Resolve command (1st argument) to rel. command enum item, allowing switch
  Commands Resolve();
};

}  // namespace docxbox

#endif  // DOCXBOX_APP_APP_COMMAND_H_
