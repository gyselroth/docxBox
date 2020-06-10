// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/app/app_command.h>

namespace docxbox {

// Constructor
AppCommands::AppCommands(std::string argc): argc_(std::move(argc)) {
  resolved_ = Resolve();
}

AppCommands::Command AppCommands::GetResolved() {
  return resolved_;
}

// Resolve name of command (1st argument) to related enum item, to allow switch
AppCommands::Command AppCommands::Resolve() {
  return ResolveCommandByName(argc_);
}

AppCommands::Command AppCommands::ResolveCommandByName(
    const std::string &command) {
  if (command == "batch") return Command_Batch;
  if (command == "cat") return Command_Cat;
  if (command == "cmd") return Command_ExecuteUserCommand;
  if (command == "diff") return Command_FileDiff;
  if (command == "h" || command == "?") return Command_Help;
  if (command == "lorem") return Command_LoremIpsum;
  if (command == "ls") return Command_List;
  if (command == "lsd") return Command_ListFields;
  if (command == "lsdj") return Command_ListFieldsAsJson;
  if (command == "lsf") return Command_ListFonts;
  if (command == "lsfj") return Command_ListFontsAsJson;
  if (command == "lsi") return Command_ListImages;
  if (command == "lsij") return Command_ListImagesAsJson;
  if (command == "lsj") return Command_ListAsJson;
  if (command == "lsl") return Command_LocateFilesContaining;
  if (command == "lslj") return Command_LocateFilesContainingAsJson;
  if (command == "lsm") return Command_ListMeta;
  if (command == "lsmj") return Command_ListMetaAsJson;
  if (command == "mm") return Command_ModifyMeta;
  if (command == "rmt") return Command_RemoveBetweenText;
  if (command == "rpi") return Command_ReplaceImage;
  if (command == "rpt") return Command_ReplaceText;
  if (command == "sfv") return Command_SetFieldValue;
  if (command == "txt") return Command_GetPlainText;
  if (command == "uz") return Command_Unzip;
  if (command == "uzi") return Command_UnzipAndIndentXml;
  if (command == "uzm") return Command_UnzipMedia;
  if (command == "v") return Command_Version;
  if (command == "zp") return Command_Zip;
  if (command == "zpc") return Command_ZipCompressed;

  return Command_Invalid;
}

bool AppCommands::IsListCommand(Command command) {
  return command == Command_List
      || command == Command_ListAsJson
      || command == Command_ListFonts
      || command == Command_ListFontsAsJson
      || command == Command_ListImages
      || command == Command_ListImagesAsJson
      || command == Command_ListFields
      || command == Command_ListFieldsAsJson
      || command == Command_ListMeta
      || command == Command_ListMetaAsJson
      || command == Command_LocateFilesContaining
      || command == Command_LocateFilesContainingAsJson;
}

bool AppCommands::IsReplaceCommand(Command command) {
  return command == Command_ReplaceText
      || command == Command_ReplaceImage
      || command == Command_RemoveBetweenText
      || command == Command_LoremIpsum
      || command == Command_SetFieldValue;
}

}  // namespace docxbox
