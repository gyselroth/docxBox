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
        has_column_titles = true;

        auto value = it.value();
        for (nlohmann::json::iterator it_header = value.begin();
             it_header != value.end();
             ++it_header) {
          column_titles_.push_back(it_header.value());
        }
      } else if (key == "content") {
        // TODO(kay): implement collect cell content
      }
    }
  }

  int x = 0;
}

bool docx_wml_renderer_table::Render() {
  if (!is_valid_table_json_) return false;

  // TODO(kay): render WML from table-properties

  return true;
}

const std::string &docx_wml_renderer_table::GetWml() const {
  return wml_;
}
