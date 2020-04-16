// Copyright (c) 2020 gyselroth GmbH

#ifndef DOCXBOX_DOCX_DOCX_ARCHIVE_H_
#define DOCXBOX_DOCX_DOCX_ARCHIVE_H_

#include <iostream>
#include <vector>
#include <memory>
#include <string>

#include <docxbox/app/app_argument.h>
#include <docxbox/docx/docx_xml_fields.h>
#include <docxbox/docx/docx_xml_lorem.h>
#include <docxbox/docx/docx_xml_replace.h>
#include <docxbox/docx/docx_xml_to_plaintext.h>

#include <docxbox/docx/docx_fontTable.h>
#include <docxbox/docx/docx_meta.h>
#include <docxbox/docx/docx_xml.h>

#include <docxbox/helper/helper_file.h>
#include <docxbox/helper/helper_string.h>

class docx_archive {
 public:
  docx_archive(int argc, char **argv);

  bool UnzipDocx(
      const std::string &directory_appendix = "",
      bool ensure_is_docx = true);

  bool UnzipMedia();
  bool Zip(std::string path_directory = "", std::string path_docx_result = "");

  bool ListFiles(bool as_json);
  bool ListFonts(bool as_json);
  bool ListImages(bool as_json);
  bool ListMergeFields(bool as_json);
  bool ListMeta(bool as_json);

  bool GetText(bool newline_at_segments);

  bool ModifyMeta();

  bool ReplaceImage();
  bool ReplaceText();
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
};

#endif  // DOCXBOX_DOCX_DOCX_ARCHIVE_H_
