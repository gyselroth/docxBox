// Copyright (c) 2020 gyselroth GmbH

#include <docxbox/app/app.h>

namespace docxbox {

// Constructor: init (resolve) command and arguments
App::App(int argc, char **argv) {
  if (argc == 1) {
    // No command given
    AppHelp::PrintHelp(true, AppCommands::Commands::Command_Invalid);

    return;
  }

  argc_ = argc;
  argv_ = argv;

  command_ = new AppCommands(argc > 0 ? argv[1] : "");
}

// Remap command + argument variations to rel. shorthand commands
AppCommands::Commands App::PreProcess(
    AppArguments *arguments,
    const AppCommands::Commands &command) const {
  switch (command) {
    case AppCommands::Command_GetPlainText:
      if (arguments->Matches(3, "-s", "--segments"))
        return AppCommands::Command_GetPlainTextSegments;

      return command;
    case AppCommands::Command_List:
      if (arguments->Matches(3, "-fj"))
        return AppCommands::Command_ListFontsAsJson;

      if (arguments->Matches(3, "-dj"))
        return AppCommands::Command_ListFieldsAsJson;

      if (arguments->Matches(3, "-ij"))
        return AppCommands::Command_ListImagesAsJson;

      if (arguments->Matches(3, "-lj"))
        return AppCommands::Command_LocateFilesContaining;

      if (arguments->Matches(3, "-mj"))
        return AppCommands::Command_ListMetaAsJson;

      if (arguments->Matches(3, "-d", "--fields"))
        return arguments->Matches(4, "-j", "--json")
               ? AppCommands::Command_ListFieldsAsJson
               : AppCommands::Command_ListFields;

      if (arguments->Matches(3, "-f", "--fonts"))
        return arguments->Matches(4, "-j", "--json")
               ? AppCommands::Command_ListFontsAsJson
               : AppCommands::Command_ListFonts;

      if (arguments->Matches(3, "-i", "--images"))
        return arguments->Matches(4, "-j", "--json")
               ? AppCommands::Command_ListImagesAsJson
               : AppCommands::Command_ListImages;

      if (arguments->Matches(3, "-l", "--locate"))
        return arguments->Matches(4, "-j", "--json")
               ? AppCommands::Command_LocateFilesContainingAsJson
               : AppCommands::Command_LocateFilesContaining;

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
      if (arguments->Matches(3, "-f", "--format"))
        return AppCommands::Command_UnzipAndIndentXml;

      return arguments->Matches(3, "-m", "--media")
               ? AppCommands::Command_UnzipMedia
               : command;
    default:return command;
  }
}

bool App::Process() {
  auto arguments = new AppArguments(argc_, argv_);

  AppCommands::Commands command = command_->GetResolved();

  // Preprocess: Remap command + argument(s) to rel. shorthand commands
  if (argc_ > 2) command = PreProcess(arguments, command);

  bool result;

  if (AppCommands::IsListCommand(command)) {
    result = ProcessList(command);
  } else if (AppCommands::IsReplaceCommand(command)) {
    result = ProcessReplace(command);
  } else {
    auto *docx_archive = new class docx_archive(argc_, argv_);

    switch (command) {
      case AppCommands::Command_ExecuteUserCommand:
        result = docx_archive->ExecuteUserCommand();
        break;
      case AppCommands::Command_FileDiff:
        result = docx_archive->ViewFilesDiff();
        break;
      case AppCommands::Command_Help: {
        AppCommands::Commands kCommand;
        std::string command_identifier;

        if (argc_ > 2) {
          kCommand = AppCommands::ResolveCommandByName(argv_[2]);
          command_identifier = argv_[2];
        } else {
          kCommand = AppCommands::Command_Invalid;
        }

        result = AppHelp::PrintHelp(true, kCommand, command_identifier);
      }
        break;
      case AppCommands::Command_GetPlainText:
        result = docx_archive->GetText(false);
        break;
      case AppCommands::Command_GetPlainTextSegments:
        result = docx_archive->GetText(true);
        break;
      case AppCommands::Command_ModifyMeta:result = docx_archive->ModifyMeta();
        break;
      case AppCommands::Command_Unzip:result = docx_archive->UnzipDocxByArgv();
        break;
      case AppCommands::Command_UnzipAndIndentXml:
        result = docx_archive->UnzipDocxByArgv("", true, true);
        break;
      case AppCommands::Command_UnzipMedia:result = docx_archive->UnzipMedia();
        break;
      case AppCommands::Command_Version:result = AppHelp::PrintVersion();
        break;
      case AppCommands::Command_Zip:result = docx_archive->Zip();
        break;
      case AppCommands::Command_ZipCompressed:result = docx_archive->Zip(true);
        break;
      case AppCommands::Command_Invalid:
      default:AppHelp::PrintUnknownArgumentMessage(argv_[1]);
        result = false;
    }

    docx_archive->RemoveTemporaryFiles();

    delete docx_archive;
  }

  delete arguments;

  return result;
}

bool App::ProcessList(AppCommands::Commands command) {
  bool result;

  auto *docx_archive = new docx_archive_list(argc_, argv_);

  switch (command) {
    case AppCommands::Command_List:result = docx_archive->ListFilesInDocx(false);
      break;
    case AppCommands::Command_ListAsJson:
      result = docx_archive->ListFilesInDocx(true);
      break;
    case AppCommands::Command_ListFields:
      result = docx_archive->ListFieldsFromXmls(false);
      break;
    case AppCommands::Command_ListFieldsAsJson:
      result = docx_archive->ListFieldsFromXmls(true);
      break;
    case AppCommands::Command_ListImages:
      result = docx_archive->ListImageFilesInDocx(false);
      break;
    case AppCommands::Command_ListImagesAsJson:
      result = docx_archive->ListImageFilesInDocx(true);
      break;
    case AppCommands::Command_ListFonts:
      result = docx_archive->ListReferencedFonts(false);
      break;
    case AppCommands::Command_ListFontsAsJson:
      result = docx_archive->ListReferencedFonts(true);
      break;
    case AppCommands::Command_ListMeta:
      result = docx_archive->ListMetaFromXmls(false);
      break;
    case AppCommands::Command_ListMetaAsJson:
      result = docx_archive->ListMetaFromXmls(true);
      break;
    case AppCommands::Command_LocateFilesContaining:
      result = docx_archive->LocateFilesContainingString();
      break;
    case AppCommands::Command_LocateFilesContainingAsJson:
      result = docx_archive->LocateFilesContainingString(true);
      break;
    case AppCommands::Command_Invalid:
    default:AppHelp::PrintUnknownArgumentMessage(argv_[1]);
      result = false;
  }

  docx_archive->RemoveTemporaryFiles();

  delete docx_archive;

  return result;
}

bool App::ProcessReplace(AppCommands::Commands command) {
  bool result;

  auto *docx_archive = new docx_archive_replace(argc_, argv_);

  switch (command) {
    case AppCommands::Command_LoremIpsum:
      result = docx_archive->ReplaceAllTextByLoremIpsum();
      break;
    case AppCommands::Command_RemoveBetweenText:
      result = docx_archive->RemoveBetweenText();
      break;
    case AppCommands::Command_ReplaceImage:
      result = docx_archive->ReplaceImage();
      break;
    case AppCommands::Command_ReplaceText:result = docx_archive->ReplaceText();
      break;
    case AppCommands::Command_SetFieldValue:
      result = docx_archive->SetFieldValue();
      break;
    case AppCommands::Command_Invalid:
    default:AppHelp::PrintUnknownArgumentMessage(argv_[1]);
      result = false;
  }

  docx_archive->RemoveTemporaryFiles();

  delete docx_archive;

  return result;
}

}  // namespace docxbox
