
#include "app.h"

namespace docxbox {

/**
 * Constructor: init (resolve) command and arguments
 */
App::App(int argc, char **argv) {
  if (argc==1) {
    // No command given
    AppHelp::PrintHelp(true, AppCommands::Command::Command_Invalid);

    return;
  }

  this->argc = argc;
  this->argv = argv;

  command_ = new AppCommands(argc > 0 ? argv[1] : "");
}

// Remap command + argument variations to rel. shorthand commands
AppCommands::Command App::PreProcess(AppArguments *arguments, AppCommands::Command &command) const {
  switch (command) {
    case AppCommands::Command_GetPlainText:
      if (arguments->Matches(3, "-s", "--segments")) return AppCommands::Command_GetPlainTextSegments;

      return command;
    case AppCommands::Command_List:
      if (arguments->Matches(3, "-fj")) return AppCommands::Command_ListFontsAsJson;

      if (arguments->Matches(3, "-ij")) return AppCommands::Command_ListImagesAsJson;

      if (arguments->Matches(3, "-mj")) return AppCommands::Command_ListMetaAsJson;


      if (arguments->Matches(3, "-f", "--fonts"))
        return arguments->Matches(4, "-j", "--json")
               ? AppCommands::Command_ListFontsAsJson
               : AppCommands::Command_ListFonts;

      if (arguments->Matches(3, "-i", "--images"))
        return arguments->Matches(4, "-j", "--json")
               ? AppCommands::Command_ListImagesAsJson
               : AppCommands::Command_ListImages;

      if (arguments->Matches(3, "-m", "--meta"))
        return arguments->Matches(4, "-j", "--json")
               ? AppCommands::Command_ListMetaAsJson
               : AppCommands::Command_ListMeta;

      return arguments->Matches(3, "-j", "--json")
        ? AppCommands::Command_ListAsJson
        : command;
    case AppCommands::Command_ListImages:
      return arguments->Matches(3, "-j", "--json")
        ? AppCommands::Command_ListImagesAsJson
        : command;
    case AppCommands::Command_ListFonts:
      return arguments->Matches(3, "-j", "--json")
        ? AppCommands::Command_ListFontsAsJson
        : command;
    case AppCommands::Command_ListMeta:
      return arguments->Matches(3, "-j", "--json")
        ? AppCommands::Command_ListMetaAsJson
        : command;
    case AppCommands::Command_Unzip:
      return arguments->Matches(3, "-m", "--media")
        ? AppCommands::Command_UnzipMedia
        : command;
    default:return command;
  }
}

bool App::Process() {
  auto *docx_archive = new class docx_archive(argc, argv);

  auto arguments = new AppArguments(argc, argv);

  AppCommands::Command command = command_->GetResolved();

  // Preprocess: Remap command + argument(s) constellations to rel. shorthand commands
  if (argc > 2) command = PreProcess(arguments, command);

  // Process
  switch (command) {
    case AppCommands::Command_Help:
      return AppHelp::PrintHelp(
          true,
          argc > 2
            ? AppCommands::ResolveCommandByName(argv[2])
            : AppCommands::Command_Invalid
      );
    case AppCommands::Command_GetPlainText:return docx_archive->GetText(false);
    case AppCommands::Command_GetPlainTextSegments:return docx_archive->GetText(true);
    case AppCommands::Command_List:return docx_archive->ListFiles(false);
    case AppCommands::Command_ListAsJson:return docx_archive->ListFiles(true);
    case AppCommands::Command_ListImages:return docx_archive->ListImages(false);
    case AppCommands::Command_ListImagesAsJson:return docx_archive->ListImages(true);
    case AppCommands::Command_ListFonts:return docx_archive->ListFonts(false);
    case AppCommands::Command_ListFontsAsJson:return docx_archive->ListFonts(true);
    case AppCommands::Command_ListMeta:return docx_archive->ListMeta(false);
    case AppCommands::Command_ListMetaAsJson:return docx_archive->ListMeta(true);
    case AppCommands::Command_ModifyMeta:return docx_archive->ModifyMeta();
    case AppCommands::Command_ReplaceImage:return docx_archive->ReplaceImage();
    case AppCommands::Command_ReplaceText:return docx_archive->ReplaceText();
    case AppCommands::Command_Unzip:return docx_archive->UnzipDocx();
    case AppCommands::Command_UnzipMedia:return docx_archive->UnzipMedia();
    case AppCommands::Command_Version:return AppHelp::PrintVersion();
    case AppCommands::Command_Zip:return docx_archive->Zip();
    case AppCommands::Command_Invalid:
    default:
      AppHelp::PrintUnknownArgumentMessage(argv[1]);
      return false;
  }
}

} // namespace docxbox
