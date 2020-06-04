// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

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
    case AppCommands::Command_Batch: return PrintHelpOnBatch();
    case AppCommands::Command_Cat: return PrintHelpOnCat();
    case AppCommands::Command_ExecuteUserCommand:
      return PrintHelpOnUserCommand();
    case AppCommands::Command_FileDiff:return PrintHelpOnDiff();
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

    case AppCommands::Command_ListFields:
    case AppCommands::Command_ListFieldsAsJson:
      return PrintHelpOnListMergeFields(true);

    case AppCommands::Command_ListMeta:
    case AppCommands::Command_ListMetaAsJson:return PrintHelpOnListMeta(true);

    case AppCommands::Command_LocateFilesContaining:
    case AppCommands::Command_LocateFilesContainingAsJson:
      return PrintHelpOnLocateFilesContaining();

    case AppCommands::Command_LoremIpsum:return PrintHelpOnLoremIpsum();

    case AppCommands::Command_RemoveBetweenText:
      return PrintHelpOnRemoveBetweenText();

    case AppCommands::Command_SetFieldValue:
      return PrintHelpOnSetFieldValue();

    case AppCommands::Command_ModifyMeta:return PrintHelpOnModifyMeta();
    case AppCommands::Command_ReplaceImage:return PrintHelpOnReplaceImage();
    case AppCommands::Command_ReplaceText:return PrintHelpOnReplaceText();

    case AppCommands::Command_Unzip:return PrintHelpOnUnzip();
    case AppCommands::Command_UnzipAndIndentXml:
      return PrintHelpOnUnzip(false, false, true);
    case AppCommands::Command_UnzipMedia:
      return PrintHelpOnUnzip(false, true, false);
    case AppCommands::Command_Version:return PrintHelpOnVersion();
    case AppCommands::Command_Zip:return PrintHelpOnZip(true);
    case AppCommands::Command_ZipCompressed:return PrintHelpOnZip(false);
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
    std::cout
      << "docxBox v" << DOCXBOX_VERSION_MAJOR << "."
      << DOCXBOX_VERSION_MINOR << "." << DOCXBOX_VERSION_PATCH
      << " - Usage: docxbox <command> [args]" << "\n\n"
      "Available commands:\n";

  std::string column_1 =
      "\n  List DOCX contents:"
      "\n    ls    - Output list of files in DOCX"
      "\n    lsj   - Output list of files in DOCX as JSON"
      "\n    lsf   - Output list of fonts in DOCX"
      "\n    lsfj  - Output list of fonts in DOCX as JSON"
      "\n    lsd   - Output list of fields in DOCX"
      "\n    lsdj  - Output list of fields in DOCX as JSON"
      "\n    lsi   - Output list of images in DOCX"
      "\n    lsij  - Output list of images in DOCX as JSON"
      "\n    lsl   - Output list of files containing given string"
      "\n    lsm   - Output DOCX meta of data"
      "\n    lsmj  - Output DOCX meta data as JSON"
      "\n"
      "\n  Convert and compare DOCX:"
      "\n    cat   - Output content of file contained within DOCX"
      "\n    txt   - Output DOCX document as plaintext"
      "\n    diff  - Side-by-side compare file from two DOCX "
      "archives"
      "\n"
      "\n  Meta commands:"
      "\n    h     - Help: Describe usage of this program"
      "\n    v     - Version: Output version number"
      "\n\n"
      "Type 'docxbox help <command>' "
      "for more help on a specific command."
      "\n\n";

  std::string column_2 =
      "\n  Manipulate DOCX document:"
      "\n    rpi   - Replace image in DOCX"
      "\n    rpt   - Replace text in DOCX"
      "\n    rmt   - Remove DOCX content between given strings"
      "\n    mm    - Modify or set meta attribute in DOCX"
      "\n    sfv   - Set field value"
      "\n    lorem - Replace all text by random dummy text"
      "\n"
      "\n  Batch process multiple docxBox commands on DOCX:"
      "\n    batch"
      "\n"
      "\n  Execute user-defined command on contained file(s) of DOCX:"
      "\n    cmd"
      "\n"
      "\n  Extract and create DOCX:"
      "\n    uz    - Unzip files from DOCX"
      "\n    uzi   - Unzip DOCX and indent XML files"
      "\n    uzm   - Unzip only media files from DOCX"
      "\n    zp    - Create (zip) DOCX from files"
      "\n    zpc   - Compress XML, than create DOCX from files";

  std::cout << helper::String::RenderTwoColumns(column_1, column_2);

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

bool AppHelp::PrintHelpOnUserCommand() {
  std::cout
    << "Command cmd - Execute given command on files(s) of given DOCX:\n"
       "--------------------------------------------------------------\n"
       "Example: Edit contained XML file w/ nano\n"
       "  docxbox cmd foo.docx \"nano *DOCX*/word/settings.xml\"\n\n";

  return true;
}

bool AppHelp::PrintHelpOnBatch() {
  std::cout
    << "Command batch - Process multiple docxBox commands upon given DOCX\n"
       "-----------------------------------------------------------------\n"
       "Example: Replace string \"foo\" by heading-1 w/ text \"Heading\",\n"
       "         than insert a heading-2 with text \"Sub-Heading\" after it\n\n"
       "  docxbox batch foo.docx "
       "{"
        "\\\"rpt\\\":["
          "\\\"foo\\\","
          "{\\\"h1\\\":{\\\"text\\\":\\\"Heading\\\",\\\"post\\\":{\\\"para\\\":\\\"my-postfix\\\"}}}"
        "],"
        "\\\"rpt\\\":["
          "\\\"my-postfix\\\","
          "{\\\"h2\\\":{\\\"text\\\":\\\"Sub-Heading\\\"}}"
        "]"
       "}\n\n";

  return true;
}

bool AppHelp::PrintHelpOnCat() {
  std::cout << "Command cat - Output content of file contained within DOCX\n"
               "----------------------------------------------------------\n"
               "docxbox cat foo.docx word/settings.xml\n\n";

  return true;
}

bool AppHelp::PrintHelpOnDiff() {
  std::cout << "Command diff - Side-by-side compare file from two DOCX "
               "archives:\n"
               "-------------------------------------------------------"
               "---------\n"
               "docxbox diff foo_v1.docx foo_v2.docx word/settings.xml\n\n";

  return true;
}

bool AppHelp::PrintHelpOnLocateFilesContaining() {
  std::cout << "List all files containing search-string or regular "
               "expression:\n"
               "--------------------------------------------------------------"
               "\n"
               "Lists all files containing the string foo:\n"
               "  docxbox lsl foo.docx foo\n"
               "  or: docxbox ls foo.docx -l foo\n"
               "  or: docxbox ls foo.docx --locate foo\n\n"

               "Lists all files containing the string foo, as JSON:"
               "  docxbox lslj foo.docx foo\n"
               "  or: docxbox lslj foo.docx foo\n"
               "  or: docxbox ls foo.docx --lj foo\n"
               "  or: docxbox lsl foo.docx --json foo\n"
               "  or: docxbox ls foo.docx --locate -j foo\n"
               "  or: docxbox ls foo.docx --locate --json foo\n\n";

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

bool AppHelp::PrintHelpOnRemoveBetweenText() {
  std::cout
    << "Command: rmt - Remove DOCX content between given strings:\n"
       "----------------------------------------------\n"
       "Replace all content between \"left-hand-side\" and "
       "\"right-hand-side\":\n"
       "  Update existing DOCX file:\n"
       "    docxbox rmt foo.docx left-hand-side right-hand-side\n\n"
       "  Save to new DOCX file:\n"
       "    docxbox rmt foo.docx left-hand-side right-hand-side new.docx\n\n";

  return true;
}

bool AppHelp::PrintHelpOnSetFieldValue() {
  std::cout
    << "Command: sfv - Set field value:\n"
       "-------------------------------\n"
       "Update shown text of all print-date fields' to 10.01.2020:"
       "  Update existing DOCX file:\n"
       "    docxbox sfv foo.docx \"PRINTDATE\" \"10.01.2020\"\n\n"
       "  Save to new DOCX file:\n"
       "    docxbox sfv foo.docx \"PRINTDATE\" \"10.01.2020\" new.docx\n\n"

       "Set shown text of all merge fields', whose identifier begins with foo, "
       "to bar:\n"
       "    docxbox sfv foo.docx \"MERGEFIELD foo\" bar\n\n"
       "  Save to new DOCX file:\n"
       "    docxbox sfv foo.docx \"MERGEFIELD foo\" bar new.docx\n\n";

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

bool AppHelp::PrintHelpOnUnzip(bool with_uz, bool with_uzm, bool with_uzi) {
  if (with_uz) {
    std::cout << "Command: uz - Unzip given DOCX file:\n"
                 "------------------------------------\n"
                 "Unzip all files from DOCX:\n"
                 "  docxbox uz foo.docx \n\n";
  }

  if (with_uzm) {
    std::cout << "Command: uzm - Unzip only media files DOCX file:\n"
                 "------------------------------------------------\n"
                 "  docxbox uzm foo.docx\n"
                 "  or: docxbox uz foo.docx -m\n"
                 "  or: docxbox uz foo.docx --media\n\n";
  }

  if (with_uzi) {
    std::cout << "Command: uzi - Unzip DOCX and indent XML files:\n"
                 "-----------------------------------------------\n"
                 "  docxbox uzi foo.docx\n"
                 "  or: docxbox uz foo.docx -i\n"
                 "  or: docxbox uz foo.docx --indent\n\n";
  }

  return true;
}

bool AppHelp::PrintHelpOnZip(bool with_zip) {
  if (with_zip) {
    std::cout << "Command: zp - Create (zip) DOCX from files:\n"
                 "--------------------------------------------\n"
                 "Create DOCX from files in given (directory) path "
                 "to new DOCX:\n"
                 "  docxbox zp foo foo.docx\n\n";
  }

  std::cout << "Command: zpc - Compress XML, than create DOCX from files:\n"
               "---------------------------------------------------------\n"
               "  docxbox zpc foo foo.docx\n\n";

  return true;
}

void AppHelp::PrintUnknownArgumentMessage(const char *arg) {
  std::cerr << "Unknown argument: \"" << arg << "\". ";
  std::cout << "Possible arguments are:\n";

  PrintHelp(false, AppCommands::Commands::Command_Invalid);
}

}  // namespace docxbox
