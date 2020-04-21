// Copyright (c) 2020 gyselroth GmbH

#ifndef DOCXBOX_DOCX_WML_RENDERER_DOCX_WML_RENDERER_TABLE_H_
#define DOCXBOX_DOCX_WML_RENDERER_DOCX_WML_RENDERER_TABLE_H_

#include <docxbox/docx/wml_renderer/docx_wml_renderer.h>
#include <docxbox/helper/helper_string.h>

#include <string>
#include <vector>

#include <vendor/json/single_include/nlohmann/json.hpp>

class docx_wml_renderer_table:docx_wml_renderer {
 public:
  // Constructor
  explicit docx_wml_renderer_table(const std::string &json);

  std::string GetWml();

  bool Render();

 private:
  // Table specs from JSON
  int amount_columns_ = 0;
  int amount_rows_ = 0;
  std::vector<std::string> column_headers_;
  std::vector<std::string> cell_content_;

  // TODO(kay): make seizing parametric and/or generic
  int table_width_ = 5000;
  int col_width_ = 2880;

  // Generic table specs
  bool has_column_headers = false;

  // Collect table specs from JSON
  void InitSpecs();

  std::string RenderTableProperties();
  std::string RenderTableGrid();
  std::string RenderTableCell(int index_cell);
  std::string RenderTableRows();
};

#endif  // DOCXBOX_DOCX_WML_RENDERER_DOCX_WML_RENDERER_TABLE_H_
