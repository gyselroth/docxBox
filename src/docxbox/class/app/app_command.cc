
#include "app_command.h"

namespace docxbox {

/**
 * Constructor
 */
AppCommands::AppCommands(std::string argc): argc_(std::move(argc)) {
  resolved_ = Resolve();
}

AppCommands::Command AppCommands::GetResolved() {
  return resolved_;
}

/**
 * Resolve name of command string (1st argument) to related enum item (which allows e.g. switch)
 */
AppCommands::Command AppCommands::Resolve() {
  return ResolveCommandByName(argc_);
}

AppCommands::Command AppCommands::ResolveCommandByName(const std::string &command) {
  if (command=="h" || command=="?") return Command_Help;
  if (command=="ls") return Command_List;
  if (command=="lsi") return Command_ListImages;
  if (command=="lsij") return Command_ListImagesAsJson;
  if (command=="lsj") return Command_ListAsJson;
  if (command=="lsf") return Command_ListFonts;
  if (command=="lsfj") return Command_ListFontsAsJson;
  if (command=="lsm") return Command_ListMeta;
  if (command=="lsmj") return Command_ListMetaAsJson;
  if (command=="mm") return Command_ModifyMeta;
  if (command=="rpi") return Command_ReplaceImage;
  if (command=="rpt") return Command_ReplaceText;
  if (command=="txt") return Command_GetPlainText;
  if (command=="uzm") return Command_UnzipMedia;
  if (command=="v") return Command_Version;
  if (command=="uz") return Command_Unzip;
  if (command=="zip") return Command_Zip;

  return Command_Invalid;
}

} // namespace docxbox