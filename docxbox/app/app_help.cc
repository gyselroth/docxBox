// Copyright (c) 2020 gyselroth GmbH

#include <docxbox/app/app_help.h>

namespace docxbox {

bool AppHelp::PrintVersion() {
  std::cout
    << "docxBox" << " version "
    << std::string(DOCXBOX_VERSION_MAJOR) << "."
    << std::string(DOCXBOX_VERSION_MINOR) << "."
    << std::string(DOCXBOX_VERSION_PATCH)
    << "\n";

  return true;
}

bool AppHelp::PrintHelp(bool with_title,
                        AppCommands::Commands command,
                        const std::string &command_identifier) {
  switch (command) {
    case AppCommands::Command_Help:return PrintOverview(true);

    case AppCommands::Command_GetPlainText:
    case AppCommands::Command_GetPlainTextSegments:
      return PrintHelpOnGetPlainText();

    case AppCommands::Command_List:
    case AppCommands::Command_ListAsJson:return PrintHelpOnList();

    case AppCommands::Command_ListImages:
    case AppCommands::Command_ListImagesAsJson:
      return PrintHelpOnListImages(true);

    case AppCommands::Command_ListFontsAsJson:
    case AppCommands::Command_ListFonts:return PrintHelpOnListFonts(true);

    case AppCommands::Command_ListMergeFields:
    case AppCommands::Command_ListMergeFieldsAsJson:
      return PrintHelpOnListMergeFields(true);

    case AppCommands::Command_ListMeta:
    case AppCommands::Command_ListMetaAsJson:return PrintHelpOnListMeta(true);

    case AppCommands::Command_LoremIpsum:return PrintHelpOnLoremIpsum();

    case AppCommands::Command_ModifyMeta:return PrintHelpOnModifyMeta();
    case AppCommands::Command_ReplaceImage:return PrintHelpOnReplaceImage();
    case AppCommands::Command_ReplaceText:return PrintHelpOnReplaceText();

    case AppCommands::Command_Unzip:return PrintHelpOnUnzip(true);
    case AppCommands::Command_UnzipMedia:return PrintHelpOnUnzip(false);

    case AppCommands::Command_Version:return PrintHelpOnVersion();
    case AppCommands::Command_Zip:return PrintHelpOnZip();
    case AppCommands::Command_Invalid:
      if (!command_identifier.empty()) {
        std::cerr << "Unknown command: " << command_identifier << ".\n\n";

        with_title = true;
      }

      return PrintOverview(with_title);
  }

  std::cout << "\n\n";

  return true;
}

bool AppHelp::PrintOverview(bool with_title) {
  if (with_title)
    std::cout << "Usage: docxbox <command> [args]" << "\n\nAvailable commands:";

  std::cout << "\n  1. List DOCX contents:"
            << "\n    ls         - Output list of files in DOCX"
            << "\n    lsj        - Output list of files in DOCX as JSON"
            << "\n    lsf        - Output list of fonts in DOCX"
            << "\n    lsg        - Output list of fields in DOCX"
            << "\n    lsgj       - Output list of fields in DOCX as JSON"
            << "\n    lsfj       - Output list of fonts in DOCX as JSON"
            << "\n    lsi        - Output list of images in DOCX"
            << "\n    lsij       - Output list of images in DOCX as JSON"
            << "\n    lsm        - Output DOCX meta of data"
            << "\n    lsmj       - Output DOCX meta data as JSON"
            << "\n"
            << "\n  2. Manipulate DOCX document:"
            << "\n    rpi        - Replace image in DOCX"
            << "\n    rpt        - Replace text in DOCX"
            << "\n    mm         - Modify or set meta attribute in DOCX"
            << "\n    lorem      - Replace all text by random dummy text"
            << "\n"
            << "\n  3. Convert DOCX:"
            << "\n    txt        - Output DOCX document as plaintext"
            << "\n"
            << "\n  4. Extract and create DOCX:"
            << "\n    uz         - Unzip files from DOCX"
            << "\n    uzm        - Unzip only media files from DOCX"
            << "\n    zip        - Create (zip) DOCX from files"
            << "\n"
            << "\n  5. Meta commands:"
            << "\n    h          - Help: Describe usage of this program"
            << "\n    v          - Version: Output version number"
            << "\n\n"
            << "Type 'docxbox help <command>' for help on a specific command."
            << "\n\n";

  return true;
}

bool AppHelp::PrintHelpOnList() {
  std::cout << "Command: ls - List DOCX contents:\n"
               "---------------------------------\n"
               "Output list of files in DOCX:\n"
               "  docxbox ls foo.docx \n\n";

  std::cout << "Output list of files in DOCX as JSON:\n"
               "  docxbox lsj foo.docx\n"
               "  or: docxbox ls foo.docx -j\n"
               "  or: docxbox txt foo.docx --json\n\n";

  PrintHelpOnListImages(false);
  PrintHelpOnListFonts(false);
  PrintHelpOnListMeta(false);

  return true;
}

bool AppHelp::PrintHelpOnListImages(bool with_title) {
  if (with_title) {
    std::cout << "Command: lsf - List images in DOCX:\n"
                 "-----------------------------------\n";
  }

  std::cout << "Output list of images in DOCX:\n"
               "  docxbox lsi foo.docx \n\n"
               "  or: docxbox ls foo.docx -i\n"
               "  or: docxbox ls foo.docx --images\n\n";

  std::cout << "Output list of images in DOCX as JSON:\n"
               "  docxbox lsij foo.docx \n\n"
               "  or: docxbox lsi foo.docx -j\n"
               "  or: docxbox lsi foo.docx --json\n"
               "  or: docxbox ls foo.docx -ij\n"
               "  or: docxbox ls foo.docx --images --json\n\n";

  return true;
}

bool AppHelp::PrintHelpOnListFonts(bool with_title) {
  if (with_title) {
    std::cout << "Command: lsf - List fonts referenced in DOCX:\n"
                 "---------------------------------------------\n";
  }

  std::cout << "Output list of fonts referenced in DOCX:\n"
               "  docxbox lsf foo.docx \n\n"
               "  or: docxbox ls foo.docx -f\n"
               "  or: docxbox ls foo.docx --fonts\n\n";

  std::cout << "Output list of fonts referenced in DOCX as JSON:\n"
               "  docxbox lsfj foo.docx \n\n"
               "  or: docxbox lsf foo.docx -j\n"
               "  or: docxbox lsf foo.docx --json\n"
               "  or: docxbox ls foo.docx -fj\n"
               "  or: docxbox ls foo.docx --fonts --json\n\n";

  return true;
}

bool AppHelp::PrintHelpOnListMeta(bool with_title) {
  if (with_title) {
    std::cout << "Command: lsm - List meta data of DOCX:\n"
                 "--------------------------------------\n";
  }

  std::cout << "Output meta data of DOCX:\n"
               "  docxbox lsm foo.docx \n\n"
               "  or: docxbox ls foo.docx -m\n"
               "  or: docxbox ls foo.docx --meta\n\n";

  std::cout << "Output meta data of DOCX as JSON:\n"
               "  docxbox lsmj foo.docx \n\n"
               "  or: docxbox lsm foo.docx -j\n"
               "  or: docxbox lsm foo.docx --json\n"
               "  or: docxbox ls foo.docx -mj\n"
               "  or: docxbox ls foo.docx --meta -- json\n\n";

  return true;
}

bool AppHelp::PrintHelpOnListMergeFields(bool with_title) {
  if (with_title) {
    std::cout << "Command: lsd - List fields from DOCX:\n"
                 "-------------------------------------\n";
  }

  std::cout << "Output list of fields from DOCX:\n"
               "  docxbox lsd foo.docx \n\n"
               "  or: docxbox ls foo.docx -g\n"
               "  or: docxbox ls foo.docx --fields\n\n";

  std::cout << "Output list of fields from DOCX as JSON:\n"
               "  docxbox lsdj foo.docx \n\n"
               "  or: docxbox lsd foo.docx -j\n"
               "  or: docxbox lsd foo.docx --json\n"
               "  or: docxbox ls foo.docx -dj\n"
               "  or: docxbox ls foo.docx --fields -- json\n\n";

  return true;
}

bool AppHelp::PrintHelpOnLoremIpsum() {
  std::cout << "Replace all text by \"Lorem Ipsum\" dummy text:\n"
               "-----------------------------------------------\n"
               "Update existing file:\n"
               "  docxbox lorem foo.docx\n\n"
               "Save to new file:"
               "  docxbox lorem foo.docx new.docx\n\n";

  return true;
}

bool AppHelp::PrintHelpOnModifyMeta() {
  std::cout << "Command: mm - Modify or set meta attribute in DOCX:\n"
               "---------------------------------------------------\n"
               "Examples for all supported attributes:\n"
               "  docxbox mm foo.docx title \"Foo bar, baz\"\n"
               "  docxbox mm foo.docx creator \"docxBox v0.0.1\"\n"
               "  docxbox mm foo.docx lastModifiedBy \"docxBox v0.0.1\"\n"
               "  docxbox mm foo.docx revision 2\n"
               "  docxbox mm foo.docx lastPrinted \"2020-01-10T10:31:00Z\"\n"
               "  docxbox mm foo.docx language \"en-US\"\n"
               "  docxbox mm foo.docx modified \"2020-01-29T09:21:00Z\"\n"
               "  docxbox mm foo.docx created \"2020-01-29T09:21:00Z\"\n\n";

  return true;
}

bool AppHelp::PrintHelpOnReplaceImage() {
  std::cout
  << "Command: rpi - Replace image in DOCX document:\n"
     "----------------------------------------------\n"
     "Replace image, update existing DOCX file:\n"
     "  docxbox rpi foo.docx img1.jpeg img/replacement.jpeg\n\n"
     "Replace image, save to new changed DOCX document:\n"
     "  docxbox rpi foo.docx img.jpeg img/replacement.jpeg new.docx\n\n";

  return true;
}

bool AppHelp::PrintHelpOnReplaceText() {
  std::cout << "Command: rpt - Replace text in DOCX document:\n"
               "----------------------------------------------\n"
               "Replace all occurrences of \"old\" by \"new\", "
               "update existing DOCX file:\n"
               "  docxbox rpt foo.docx old new\n\n"
               "Replace all occurrences of \"old\" by \"new\", "
               "save to new DOCX file:\n"
               "  docxbox rpt foo.docx old new new.docx\n\n";

  return true;
}

bool AppHelp::PrintHelpOnGetPlainText() {
  std::cout << "Command: txt - Output plaintext from DOCX document:\n"
               "---------------------------------------------------\n"
               "Output plaintext from DOCX document:\n"
               "  docxbox txt foo.docx \n\n";
  std::cout << "Output plaintext segments from DOCX document:\n"
               "  docxbox txt foo.docx --segments\n"
               "  or: docxbox txt foo.docx -s\n\n";

  return true;
}

bool AppHelp::PrintHelpOnVersion() {
  std::cout << "Command: v - Output docxBox version number:\n"
               "-------------------------------------------\n"
               "  docxbox v\n\n";

  return true;
}

bool AppHelp::PrintHelpOnUnzip(bool with_uz) {
  if (with_uz) {
    std::cout << "Command: uz - Unzip given DOCX file:\n"
                 "------------------------------------\n"
                 "Unzip all files from DOCX:\n"
                 "  docxbox uz foo.docx \n\n"
                 "Output only media files from DOCX document:\n";
  } else {
    std::cout << "Command: uzm - Unzip only media files DOCX file:\n"
                 "------------------------------------------------\n";
  }

  std::cout << "  docxbox uzm foo.docx\n"
               "  or: docxbox uz foo.docx -m\n"
               "  or: docxbox uz foo.docx --media\n\n";

  return true;
}

bool AppHelp::PrintHelpOnZip() {
  std::cout << "Command: zip - Create (zip) DOCX from files:\n"
               "--------------------------------------------\n"
               "Create DOCX from files in given (directory) path to new DOCX:\n"
               "  docxbox zip foo foo.docx\n\n";

  return true;
}

void AppHelp::PrintUnknownArgumentMessage(const char *arg) {
  std::cerr << "Unknown argument: \"" << arg << "\".";
  std::cout << "Possible arguments are:\n";

  PrintHelp(false, AppCommands::Commands::Command_Invalid);
}

}  // namespace docxbox
