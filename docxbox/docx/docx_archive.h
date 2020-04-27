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

#include <docxbox/ext/ext_miniz_cpp.hpp>

class docx_archive {
 public:
  docx_archive(int argc, char **argv);

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

  bool GetText(bool newline_at_segments);

  bool ExecuteUserCommand();

  bool ViewFilesDiff();

  bool ModifyMeta();

 protected:
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

  std::string ParseFileWildcard(int index_argument) const;

 private:
  // Update given meta date attribute and immediately save updated core.xml
  bool UpdateCoreXmlDate(docx_meta::Attribute attribute);
};

#endif  // DOCXBOX_DOCX_DOCX_ARCHIVE_H_
