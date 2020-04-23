// Copyright (c) 2020 gyselroth GmbH

#include <docxbox/app/app_command.h>

namespace docxbox {

// Constructor
AppCommands::AppCommands(std::string argc): argc_(std::move(argc)) {
  resolved_ = Resolve();
}

AppCommands::Commands AppCommands::GetResolved() {
  return resolved_;
}

// Resolve name of command (1st argument) to related enum item, to allow switch
AppCommands::Commands AppCommands::Resolve() {
  return ResolveCommandByName(argc_);
}

AppCommands::Commands AppCommands::ResolveCommandByName(
    const std::string &command) {
  if (command == "h" || command == "?") return Command_Help;
  if (command == "lorem") return Command_LoremIpsum;
  if (command == "ls") return Command_List;
  if (command == "lsd") return Command_ListMergeFields;
  if (command == "lsdj") return Command_ListMergeFieldsAsJson;
  if (command == "lsi") return Command_ListImages;
  if (command == "lsij") return Command_ListImagesAsJson;
  if (command == "lsj") return Command_ListAsJson;
  if (command == "lsf") return Command_ListFonts;
  if (command == "lsfj") return Command_ListFontsAsJson;
  if (command == "lsm") return Command_ListMeta;
  if (command == "lsmj") return Command_ListMetaAsJson;
  if (command == "mm") return Command_ModifyMeta;
  if (command == "rem") return Command_RemoveBetweenText;
  if (command == "rpi") return Command_ReplaceImage;
  if (command == "rpt") return Command_ReplaceText;
  if (command == "txt") return Command_GetPlainText;
  if (command == "uz") return Command_Unzip;
  if (command == "uzi") return Command_UnzipAndIndentXml;
  if (command == "uzm") return Command_UnzipMedia;
  if (command == "v") return Command_Version;
  if (command == "zip") return Command_Zip;

  return Command_Invalid;
}

}  // namespace docxbox
