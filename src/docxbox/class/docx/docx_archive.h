
#ifndef CLASS_DOCXBOX_DOCX_ARCHIVE_H
#define CLASS_DOCXBOX_DOCX_ARCHIVE_H

#include <string>
#include <iostream>

#include "../app/app_argument.h"
#include "docx_xml_to_plaintext.h"
#include "docx_fontTable.h"
#include "docx_meta.h"
#include "docx_xml.h"

#include "../../helper/helper_file.h"
#include "../../helper/helper_string.h"

class docx_archive {
 public:
  docx_archive(int argc, char **argv);

  bool UnzipDocx(const std::string& directory_appendix = "", bool ensure_is_docx = true);
  bool UnzipMedia();
  bool Zip(std::string path_directory = "", std::string path_docx_result = "");

  bool ListFiles(bool as_json);
  bool ListFonts(bool as_json);
  bool ListImages(bool as_json);
  bool ListMeta(bool as_json);

  bool GetText(bool newline_at_segments);

  bool ModifyMeta();

  bool ReplaceImage();
  bool ReplaceImages();
  bool ReplaceText();

 private:
  int argc;
  char **argv;

  std::string path_working_directory;

  // DOCX to be read or manipulated
  std::string path_docx_in;
  // Directory containing extracted XML files of DOCX
  std::string path_extract;

  bool InitPathDocxByArgV(int index_path_argument);
  void InitExtractionPath(const std::string &directory_appendix, const std::string &path_docx);
};

#endif //CLASS_DOCXBOX_DOCX_ARCHIVE_H
