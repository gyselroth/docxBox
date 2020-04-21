// Copyright (c) 2020 gyselroth GmbH

#include <docxbox/docx/wml_renderer/docx_wml_renderer_table.h>

#include <iostream>

docx_wml_renderer_table::docx_wml_renderer_table(const std::string &json) {
  json_ = json;
  is_valid_table_json_ = helper::String::IsJson(json);

  if (is_valid_table_json_) InitSpecs();
}

// Collect table specs from JSON
void docx_wml_renderer_table::InitSpecs() {
  if (!helper::String::Contains(json_, "table")) {
    is_valid_table_json_ = false;

    return;
  }

  auto json_outer = nlohmann::json::parse(json_);

  for (auto& json_inner : json_outer) {
    for (nlohmann::json::iterator it = json_inner.begin();
         it != json_inner.end();
         ++it) {
      const std::string& key = it.key();

      if (key == "columns") {
        amount_columns_ = it.value();
      } else if (key == "rows") {
        amount_rows_ = it.value();
      } else if (key == "header") {
        has_column_headers = true;
        amount_columns_ = 0;

        auto headers = it.value();

        for (nlohmann::json::iterator it_header = headers.begin();
             it_header != headers.end();
             ++it_header) {
          column_headers_.push_back(it_header.value());

          ++amount_columns_;
        }
      } else if (key == "content") {
        amount_rows_ = 0;

        auto rows = it.value();

        for (nlohmann::json::iterator it_row = rows.begin();
             it_row != rows.end();
             ++it_row) {
          auto row = it_row.value();

          for (nlohmann::json::iterator it_column = row.begin();
               it_column != row.end();
               ++it_column) {
            cell_content_.push_back(it_column.value());
          }

          ++amount_rows_;
        }
      }
    }
  }

  is_valid_table_json_ = amount_columns_ > 0 && amount_rows_ > 0;
}

// @see http://officeopenxml.com/WPtable.php
bool docx_wml_renderer_table::Render() {
  if (!is_valid_table_json_) return false;

  wml_ = std::string(kXmlDeclaration);
  wml_ += std::string(kWRunLhs);

  wml_ += std::string(kWTableLhs);
  wml_ += RenderTableProperties();
  wml_ += RenderTableGrid();
  // TODO(kay): add rendering of header
  wml_ += RenderTableRows();
  wml_ += std::string(kWTableRhs);

  wml_ += std::string(kWRunRhs);

  return true;
}

std::string docx_wml_renderer_table::RenderTableProperties() {
  return
    "<w:tblPr>"
      "<w:tblStyle w:val=\"TableGrid\"/>"
      "<w:tblW w:w=\"" + std::to_string(table_width_) + "\" w:type=\"pct\"/>"
    "</w:tblPr>";
}

std::string docx_wml_renderer_table::RenderTableGrid() {
  std::string markup = "<w:tblGrid>";

  for (int i = 0; i < amount_columns_; i++) {
    markup += "<w:gridCol w:w=\"" + std::to_string(col_width_) + "\"/>";
  }

  return markup + "</w:tblGrid>";;
}

std::string docx_wml_renderer_table::RenderTableRows() {
  std::string markup;

  int index_cell = 0;

  for (int i = 0; i < amount_rows_; i++) {
    markup += "<w:tr>";

    for (int j = 0; j < amount_columns_; j++) {
      markup += RenderTableCell(index_cell);
    }

    markup += "</w:tr>";

    index_cell++;
  }

  return markup;
}

std::string docx_wml_renderer_table::RenderTableCell(int index_cell) {
  return
      "<w:tc>"
        "<w:tcPr>"
          "<w:tcW w:w=\"" + std::to_string(col_width_) + "\" w:type=\"dxa\"/>"
        "</w:tcPr>"
        "<w:p>"
          "<w:r>"
          "<w:t>" + cell_content_[index_cell] + "</w:t>"
          "</w:r>"
        "</w:p>"
      "</w:tc>";
}

const std::string &docx_wml_renderer_table::GetWml() const {
  return wml_;
}
