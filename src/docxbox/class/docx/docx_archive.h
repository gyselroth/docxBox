
#ifndef CLASS_DOCXBOX_DOCX_ARCHIVE_H
#define CLASS_DOCXBOX_DOCX_ARCHIVE_H

#include <string>
#include <iostream>

#include "../app/app_arguments.h"
#include "docx_fontTable.h"
#include "docx_meta.h"
#include "docx_xml.h"

#include "../../helper/helper_file.h"
#include "../../helper/helper_string.h"

class docx_archive {
 public:
  docx_archive(int argc, char **argv);

  bool Unzip(const std::string& directory_appendix = "");
  bool UnzipMedia();
  bool Zip(std::string path_directory = "", std::string path_docx_result = "");

  bool ListFiles(bool asJson);
  bool ListFonts(bool asJson);
  bool ListImages(bool asJson);
  bool ListMeta(bool asJson);

  bool GetText(bool newlineAtSegments);
  bool ReplaceImage();
  bool ReplaceImages();
  bool ReplaceText();

 private:
  int argc;
  char **argv;

  std::string path_working_directory;

  std::string path_docx;
  std::string path_extract;

  void InitPathDocxByArgV(int index_path_argument);
  void InitExtractionPath(const std::string &directory_appendix, const std::string &path_docx);
};

#endif //CLASS_DOCXBOX_DOCX_ARCHIVE_H
