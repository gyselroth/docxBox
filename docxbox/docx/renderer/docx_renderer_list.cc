// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/docx/renderer/docx_renderer_list.h>

#include <iostream>

// Constructor
docx_renderer_list::docx_renderer_list(
    int argc, char **argv, const std::string &json) {
  argc_ = argc;
  argv_ = argv;

  json_ = json;
  is_json_valid_markup_config_ = helper::String::IsJson(json);

  if (is_json_valid_markup_config_) InitSpecsFromJson();
}

void docx_renderer_list::SetIsOrdered(bool is_ordered) {
  is_ordered_ = is_ordered;
}

void docx_renderer_list::InitSpecsFromJson() {
  if (!docx_renderer::IsValidJsonForList(json_)) {
    is_json_valid_markup_config_ = false;

    return;
  }

  auto json_outer = nlohmann::json::parse(json_);

  for (auto &json_inner : json_outer) {
    for (nlohmann::json::iterator it = json_inner.begin();
         it != json_inner.end();
         ++it) {
      const std::string &key = it.key();

      if (key == "items") {
        auto items = it.value();

        for (nlohmann::json::iterator it_items = items.begin();
             it_items != items.end();
             ++it_items) items_.push_back(it_items.value());
      }
    }
  }
}

std::string docx_renderer_list::Render() {
  if (!is_json_valid_markup_config_) throw "Failed render list markup.\n";

  wml_ = kWRunLhs;

  for (const std::string& item : items_) {
    wml_ +=
        "<w:p>"
          "<w:pPr>"
            "<w:numPr>"
              "<w:ilvl w:val=\"0\"/>"
              "<w:numId w:val=\"1\"/>"
            "</w:numPr>"
            "<w:ind w:left=\"360\" w:hanging=\"360\"/>"
          "</w:pPr>"
          "<w:r>"
            "<w:t>" + item + "</w:t>"
          "</w:r>"
        "</w:p>";
  }

  wml_ += kWRunRhs;

  return wml_;
}
