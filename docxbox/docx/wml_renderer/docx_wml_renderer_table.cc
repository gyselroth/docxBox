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

bool docx_wml_renderer_table::Render() {
  if (!is_valid_table_json_) return false;

  width_ = 5000;

  wml_ = "<w:tbl>";
  wml_ += RenderTableProperties();
  wml_ += RenderTableGrid();

  return true;
}

std::string docx_wml_renderer_table::RenderTableProperties() {
  return
    "<w:tblPr>"
      "<w:tblStyle w:val=\"TableGrid\"/>"
      "<w:tblW w:w=\"" + std::to_string(width_) + "\" w:type=\"pct\"/>"
    "</w:tblPr>";
}

std::string docx_wml_renderer_table::RenderTableGrid() {
  std::string markup = "<w:tblGrid>";

  // TODO(kay): calculate width per column

  for (int i = 0; i < amount_columns_; i++) {
    markup += "<w:gridCol w:w=\"2880\"/>";
  }

  return markup + "</w:tblGrid>";;
}

const std::string &docx_wml_renderer_table::GetWml() const {
  return wml_;
}
