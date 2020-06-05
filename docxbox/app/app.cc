// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/app/app.h>

namespace docxbox {

// Constructor: init (resolve) command and arguments
App::App(int argc, char **argv, bool is_batch_mode) {
  is_batch_mode_ = is_batch_mode;

  if (!is_batch_mode && argc == 1) {
    // No command given
    AppHelp::PrintHelp(true, AppCommands::Command::Command_Invalid);

    return;
  }

  argc_ = argc;
  argv_ = argv;

  command_ = new AppCommands(argc > 0 ? argv[1] : "");
}

// Remap command + argument variations to rel. shorthand commands
AppCommands::Command App::PreProcess(
    AppArguments *arguments,
    const AppCommands::Command &command) const {
  switch (command) {
    case AppCommands::Command_GetPlainText:  // txt
      if (arguments->Matches(3, "-s", "--segments"))
        return AppCommands::Command_GetPlainTextSegments;

      return command;
    case AppCommands::Command_List:  // ls
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
    case AppCommands::Command_ListImages:  // lsi
      return arguments->Matches(3, "-j", "--json")
               ? AppCommands::Command_ListImagesAsJson
               : command;
    case AppCommands::Command_ListFonts:  // lsf
      return arguments->Matches(3, "-j", "--json")
               ? AppCommands::Command_ListFontsAsJson
               : command;
    case AppCommands::Command_LocateFilesContaining:  // lsl
      return arguments->Matches(3, "-j", "--json")
               ? AppCommands::Command_LocateFilesContainingAsJson
               : command;
    case AppCommands::Command_ListMeta:  // lsm
      return arguments->Matches(3, "-j", "--json")
               ? AppCommands::Command_ListMetaAsJson
               : command;
    case AppCommands::Command_Unzip:  // uz
      if (arguments->Matches(3, "-f", "--format"))
        return AppCommands::Command_UnzipAndIndentXml;

      if (arguments->Matches(3, "-i", "--indent"))
        return AppCommands::Command_UnzipAndIndentXml;

      return arguments->Matches(3, "-m", "--media")
               ? AppCommands::Command_UnzipMedia
               : command;
    default:return command;
  }
}

bool App::Process() {
  auto arguments = new AppArguments(argc_, argv_);

  AppCommands::Command command = command_->GetResolved();

  // Preprocess: Remap command + argument(s) to rel. shorthand commands
  if (!is_batch_mode_ && argc_ > 2) command = PreProcess(arguments, command);

  bool result;

  if (AppCommands::IsListCommand(command)) {
    result = ProcessList(command);
  } else if (AppCommands::IsReplaceCommand(command)) {
    result = ProcessReplace(command);
  } else {
    auto *docx_archive = new class docx_archive(argc_, argv_, is_batch_mode_);

    switch (command) {
      case AppCommands::Command_Batch:  // batch
        result = docx_archive->Batch();
        break;
      case AppCommands::Command_Cat:  // cat
        result = docx_archive->CatFile();
        break;
      case AppCommands::Command_ExecuteUserCommand:  // cmd
        result = docx_archive->ExecuteUserCommand();
        break;
      case AppCommands::Command_FileDiff:  // diff
        result = docx_archive->ViewFilesDiff();
        break;
      case AppCommands::Command_Help: {  // h
        AppCommands::Command kCommand;
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
      case AppCommands::Command_GetPlainText:  // txt
        result = docx_archive->GetText(false);
        break;
      case AppCommands::Command_GetPlainTextSegments:  // txt file.docx -s
        result = docx_archive->GetText(true);
        break;
      case AppCommands::Command_ModifyMeta:  // mm
        result = docx_archive->ModifyMeta();
        break;
      case AppCommands::Command_Unzip:  // uz
        result = docx_archive->UnzipDocxByArgv();
        break;
      case AppCommands::Command_UnzipAndIndentXml:  // uzi
        result = docx_archive->UnzipDocxByArgv(false, "", true, true);
        break;
      case AppCommands::Command_UnzipMedia:  // uzm
        result = docx_archive->UnzipMedia();
        break;
      case AppCommands::Command_Version:  // v
        result = AppHelp::PrintVersion();
        break;
      case AppCommands::Command_Zip:  // zp
        result = docx_archive->Zip();
        break;
      case AppCommands::Command_ZipCompressed:  // zpc
        result = docx_archive->Zip(true);
        break;
      case AppCommands::Command_Invalid:
      default:AppHelp::PrintUnknownArgumentMessage(argv_[1]);
        result = false;
    }

    delete docx_archive;
  }

  delete arguments;

  return result;
}

bool App::ProcessList(AppCommands::Command command) {
  bool result;

  auto *docx_archive = new docx_archive_list(argc_, argv_);

  switch (command) {
    case AppCommands::Command_List:  // ls
      result = docx_archive->ListFilesInDocx(false);
      break;
    case AppCommands::Command_ListAsJson:  // lsj
      result = docx_archive->ListFilesInDocx(true);
      break;
    case AppCommands::Command_ListFields:  // lsd
      result = docx_archive->ListFieldsFromXmls(false);
      break;
    case AppCommands::Command_ListFieldsAsJson:  // lsdj
      result = docx_archive->ListFieldsFromXmls(true);
      break;
    case AppCommands::Command_ListImages:  // lsi
      result = docx_archive->ListImageFilesInDocx(false);
      break;
    case AppCommands::Command_ListImagesAsJson:  // lsij
      result = docx_archive->ListImageFilesInDocx(true);
      break;
    case AppCommands::Command_ListFonts:  // lsf
      result = docx_archive->ListReferencedFonts(false);
      break;
    case AppCommands::Command_ListFontsAsJson:  // lsfj
      result = docx_archive->ListReferencedFonts(true);
      break;
    case AppCommands::Command_ListMeta:  // lsm
      result = docx_archive->ListMetaFromXmls(false);
      break;
    case AppCommands::Command_ListMetaAsJson:  // lsmj
      result = docx_archive->ListMetaFromXmls(true);
      break;
    case AppCommands::Command_LocateFilesContaining:  // lsl
      result = docx_archive->LocateFilesContainingString();
      break;
    case AppCommands::Command_LocateFilesContainingAsJson:  // lslj
      result = docx_archive->LocateFilesContainingString(true);
      break;
    case AppCommands::Command_Invalid:
    default:AppHelp::PrintUnknownArgumentMessage(argv_[1]);
      result = false;
  }

  delete docx_archive;

  return result;
}

bool App::ProcessReplace(AppCommands::Command command) {
  bool result;

  auto *docx_archive = new docx_archive_replace(argc_, argv_, is_batch_mode_);

  switch (command) {
    case AppCommands::Command_LoremIpsum:  // lorem
      result = docx_archive->ReplaceAllTextByLoremIpsum();
      break;
    case AppCommands::Command_RemoveBetweenText:  // rmt
      result = docx_archive->RemoveBetweenText();
      break;
    case AppCommands::Command_ReplaceImage:  // rpi
      result = docx_archive->ReplaceImage();
      break;
    case AppCommands::Command_ReplaceText:  // rpt
      result = docx_archive->ReplaceText();
      break;
    case AppCommands::Command_SetFieldValue:  // sfv
      result = docx_archive->SetFieldValue();
      break;
    case AppCommands::Command_Invalid:
    default:AppHelp::PrintUnknownArgumentMessage(argv_[1]);
      result = false;
  }

  delete docx_archive;

  return result;
}

}  // namespace docxbox
