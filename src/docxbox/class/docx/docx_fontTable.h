
#ifndef CLASS_DOCXBOX_DOCX_FONTTABLE_H
#define CLASS_DOCXBOX_DOCX_FONTTABLE_H

#include <vector>
#include <string>
#include <iostream>
#include <cstring>

#include "../../helper/helper_numeric.h"
#include "../../helper/helper_string.h"

class docx_fontTable {
 public:
  docx_fontTable();

  void CollectFontsMetrics(std::string fontTable_xml);
  void OutputPlain(const std::string& path_xml_file);
  void OutputAsJson(const std::string& path_xml_file);
  void Clear();

 private:
  int amount_fonts = 0;

  std::vector<std::string> font_names;
  std::vector<std::string> alt_names;
  std::vector<std::string> charsets;
  std::vector<std::string> families;
  std::vector<std::string> pitches;

  unsigned long max_len_font_name;
  unsigned long max_len_alt_name;
  unsigned long max_len_charset;
  unsigned long max_len_family;
  unsigned long max_len_pitch;

  void InitCliOutputPadding();
};

#endif //CLASS_DOCXBOX_DOCX_FONTTABLE_H
