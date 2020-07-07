// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_APP_APP_COMMAND_H_
#define DOCXBOX_APP_APP_COMMAND_H_

#include <string>
#include <utility>

namespace docxbox {

class AppCommands {
 public:
  enum Command {
    Command_Batch,
    Command_Cat,
    Command_ExecuteUserCommand,
    Command_FileDiff,
    Command_Help,
    Command_GetPlainText,
    Command_GetPlainTextSegments,
    Command_List,
    Command_ListAsJson,
    Command_ListImages,
    Command_ListImagesAsJson,
    Command_ListFonts,
    Command_ListFontsAsJson,
    Command_ListFields,
    Command_ListFieldsAsJson,
    Command_ListMeta,
    Command_ListMetaAsJson,
    Command_LocateFilesContaining,
    Command_LocateFilesContainingAsJson,
    Command_LoremIpsum,
    Command_ModifyMeta,
    Command_RemoveBetweenText,
    Command_ReplaceImage,
    Command_ReplaceText,
    Command_SetFieldValue,
    Command_SetTableValues,
    Command_Unzip,
    Command_UnzipAndIndentXml,
    Command_UnzipMedia,
    Command_Version,
    Command_Zip,
    Command_ZipCompressed,
    Command_Invalid
  };

  explicit AppCommands(std::string argc);

  static Command ResolveCommandByName(const std::string &command);

  static bool IsListCommand(Command command);
  static bool IsReplaceCommand(Command command);

  Command GetResolved();

 private:
  // Argument value as received from CLI
  std::string argc_;

  // Argument resolved to Commands-enum
  Command resolved_;

  // Resolve command (1st argument) to rel. command enum item, allowing switch
  Command Resolve();
};

}  // namespace docxbox

#endif  // DOCXBOX_APP_APP_COMMAND_H_
