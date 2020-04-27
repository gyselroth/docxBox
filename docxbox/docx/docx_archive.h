// Copyright (c) 2020 gyselroth GmbH

#ifndef DOCXBOX_DOCX_DOCX_ARCHIVE_H_
#define DOCXBOX_DOCX_DOCX_ARCHIVE_H_

#include <docxbox/app/app_argument.h>
#include <docxbox/docx/docx_fontTable.h>
#include <docxbox/docx/docx_meta.h>
#include <docxbox/docx/renderer/docx_renderer_table.h>
#include <docxbox/docx/xml/docx_xml_fields.h>
#include <docxbox/docx/xml/docx_xml_lorem.h>
#include <docxbox/docx/xml/docx_xml_remove.h>
#include <docxbox/docx/xml/docx_xml_replace.h>
#include <docxbox/docx/xml/docx_xml_to_plaintext.h>
#include <docxbox/docx/xml/docx_xml.h>
#include <docxbox/helper/helper_cli.h>
#include <docxbox/helper/helper_dateTime.h>
#include <docxbox/helper/helper_file.h>
#include <docxbox/helper/helper_string.h>

#include <iostream>
#include <vector>
#include <memory>
#include <string>

class docx_archive {
 public:
  docx_archive(int argc, char **argv);

  bool ExecuteUserCommand();
  bool LocateFilesContainingString(bool as_json = false);

  bool ViewFilesDiff();

  bool UnzipDocx(
      const std::string &directory_appendix = "",
      bool ensure_is_docx = true,
      bool format_xml_files = false);

  // Check formal structure of DOCX archive - mandatory files given?
  bool IsDocx();

  bool UnzipMedia();

  // Zip files into given path into DOCX of given filename
  // Optionally update "creation" and "modified" meta attributes (core.xml)
  // to current date-time value
  bool Zip(
      bool compress_xml = false,
      std::string path_directory = "",
      std::string path_docx_result = "",
      bool update_created = false,
      bool update_modified = false);

  // List files inside DOCX archive and their attributes
  bool ListFiles(bool as_json, bool images_only = false);
  bool ListImages(bool as_json);

  // List data stored within XML(s) of DOCX
  bool ListFonts(bool as_json);
  bool ListMergeFields(bool as_json);
  bool ListMeta(bool as_json);

  bool GetText(bool newline_at_segments);

  bool ModifyMeta();

  bool ReplaceImage();
  bool ReplaceText();
  bool RemoveBetweenText();
  bool ReplaceAllTextByLoremIpsum();

 private:
  int argc_;
  char **argv_;

  std::string path_working_directory_;

  // DOCX to be read or manipulated
  std::string path_docx_in_;

  // Directory containing extracted XML files of DOCX
  std::string path_extract_;

  bool InitPathDocxByArgV(int index_path_argument);

  void InitExtractionPath(const std::string &directory_appendix,
                          const std::string &path_docx);

  void InitLocateFilesContaining(bool &as_json, std::string &needle) const;

  std::string ParseFileWildcard(int index_argument) const;

  // Update given meta date attribute and immediately save updated core.xml
  bool UpdateCoreXmlDate(docx_meta::Attribute attribute);
};

#endif  // DOCXBOX_DOCX_DOCX_ARCHIVE_H_
