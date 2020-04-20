// Copyright (c) 2020 gyselroth GmbH

#ifndef DOCXBOX_DOCX_WML_RENDERER_DOCX_WML_RENDERER_TABLE_H_
#define DOCXBOX_DOCX_WML_RENDERER_DOCX_WML_RENDERER_TABLE_H_

#include <docxbox/docx/wml_renderer/docx_wml_renderer_table.h>
#include <docxbox/helper/helper_string.h>

#include <vendor/json/single_include/nlohmann/json.hpp>

#include <string>
#include <vector>

class docx_wml_renderer_table {
 public:
  // Constructor
  explicit docx_wml_renderer_table(const std::string &json);

  bool Render();

  const std::string &GetWml() const;

 private:
  bool is_valid_table_json_;

  std::string json_;

  int amount_columns_;
  int amount_rows_;
  bool has_column_titles = false;
  std::vector<std::string> column_titles_;
  std::vector<std::string> cell_content_;

  std::string wml_;

  // Collect table specs from JSON
  void InitSpecs();
};

#endif  // DOCXBOX_DOCX_WML_RENDERER_DOCX_WML_RENDERER_TABLE_H_
