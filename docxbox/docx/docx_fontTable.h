// Copyright (c) 2020 gyselroth GmbH

#ifndef DOCXBOX_DOCX_DOCX_FONTTABLE_H_
#define DOCXBOX_DOCX_DOCX_FONTTABLE_H_

#include <docxbox/helper/helper_numeric.h>
#include <docxbox/helper/helper_string.h>

#include <cstring>
#include <iostream>
#include <string>
#include <vector>

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

  u_int32_t max_len_font_name_;
  u_int32_t max_len_alt_name_;
  u_int32_t max_len_charset_;
  u_int32_t max_len_family_;
  u_int32_t max_len_pitch_;

  void InitCliOutputPadding();
};

#endif  // DOCXBOX_DOCX_DOCX_FONTTABLE_H_
