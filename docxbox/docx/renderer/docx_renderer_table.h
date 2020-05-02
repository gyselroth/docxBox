// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_DOCX_RENDERER_DOCX_RENDERER_TABLE_H_
#define DOCXBOX_DOCX_RENDERER_DOCX_RENDERER_TABLE_H_

#include <docxbox/docx/renderer/docx_renderer.h>

#include <string>
#include <vector>

#include <vendor/json/single_include/nlohmann/json.hpp>

class docx_renderer_table: docx_renderer {
 public:
  // Constructor
  explicit docx_renderer_table(int argc, char **argv, const std::string &json);

  std::string GetWml();

  static std::string RenderMarkup(
      int argc,
      char **argv,
      const std::string& json);

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
  std::string RenderTableRowsAndCells();
};

#endif  // DOCXBOX_DOCX_RENDERER_DOCX_RENDERER_TABLE_H_
