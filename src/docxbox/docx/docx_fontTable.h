// Copyright (c) 2020 gyselroth GmbH

#ifndef SRC_DOCXBOX_DOCX_FONTTABLE_H_
#define SRC_DOCXBOX_DOCX_FONTTABLE_H_

#include <vector>
#include <string>
#include <iostream>
#include <cstring>

#include "../helper/helper_numeric.h"
#include "../helper/helper_string.h"

class docx_fontTable {
 public:
  docx_fontTable();

  void CollectFontsMetrics(std::string fontTable_xml);
  void OutputPlain(const std::string& path_xml_file);
  void OutputAsJson(const std::string& path_xml_file);
  void Clear();

 private:
  int amount_fonts_ = 0;

  std::vector<std::string> font_names_;
  std::vector<std::string> alt_names_;
  std::vector<std::string> charsets_;
  std::vector<std::string> families_;
  std::vector<std::string> pitches_;

  unsigned long max_len_font_name_;
  unsigned long max_len_alt_name_;
  unsigned long max_len_charset_;
  unsigned long max_len_family_;
  unsigned long max_len_pitch_;

  void InitCliOutputPadding();
};

#endif  // SRC_DOCXBOX_DOCX_FONTTABLE_H_
