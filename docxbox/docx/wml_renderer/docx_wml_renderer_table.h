// Copyright (c) 2020 gyselroth GmbH

#ifndef DOCXBOX_DOCX_WML_RENDERER_DOCX_WML_RENDERER_TABLE_H_
#define DOCXBOX_DOCX_WML_RENDERER_DOCX_WML_RENDERER_TABLE_H_

#include <docxbox/docx/wml_renderer/docx_wml_renderer_table.h>
#include <docxbox/helper/helper_string.h>

#include <string>
#include <vector>

#include <vendor/json/single_include/nlohmann/json.hpp>


class docx_wml_renderer_table {
 public:
  // Constructor
  explicit docx_wml_renderer_table(const std::string &json);

  bool Render();

  const std::string &GetWml() const;

 private:
  bool is_valid_table_json_;

  std::string json_;

  // Table specs from JSON
  int amount_columns_ = 0;
  int amount_rows_ = 0;
  std::vector<std::string> column_headers_;
  std::vector<std::string> cell_content_;

  int width_ = 5000;

  // Generic table specs
  bool has_column_headers = false;


  std::string wml_;

  // Collect table specs from JSON
  void InitSpecs();

  std::string RenderTableProperties();
  std::string RenderTableGrid();
};

#endif  // DOCXBOX_DOCX_WML_RENDERER_DOCX_WML_RENDERER_TABLE_H_
