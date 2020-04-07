
#ifndef CLASS_DOCXBOX_APP_COMMAND
#define CLASS_DOCXBOX_APP_COMMAND

#include <string>

namespace docxbox {

class AppCommands {
 public:
  enum Command {
    Command_Help,
    Command_GetPlainText,
    Command_GetPlainTextSegments,
    Command_List,
    Command_ListAsJson,
    Command_ListImages,
    Command_ListImagesAsJson,
    Command_ListFonts,
    Command_ListFontsAsJson,
    Command_ListMeta,
    Command_ListMetaAsJson,
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

  static Command ResolveCommandByName(const std::string &command);

  Command GetResolved();

 private:
  // Argument value as received from CLI
  std::string argc_;

  // Argument resolved to Commands-enum
  Command resolved_;

  // Resolve command string (1st argument) to related command enum item (which allows e.g. switch)
  Command Resolve();
};

} // namespace docxbox

#endif // CLASS_DOCXBOX_APP_COMMAND