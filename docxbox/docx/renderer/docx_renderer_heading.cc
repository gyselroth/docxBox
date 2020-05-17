// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/docx/renderer/docx_renderer_heading.h>

#include <iostream>

// Constructor
docx_renderer_heading::docx_renderer_heading(
    int argc,
    char **argv,
    const std::string &json) {
  argc_ = argc;
  argv_ = argv;

  json_ = json;
  is_json_valid_markup_config_ = helper::String::IsJson(json);

  if (is_json_valid_markup_config_) InitSpecsFromJson();
}

void docx_renderer_heading::SetLevel(int level) {
  level_ = level;
}

void docx_renderer_heading::InitSpecsFromJson() {
  if (!docx_renderer::IsValidJsonForHeading(json_)) {
    is_json_valid_markup_config_ = false;

    return;
  }

  auto json_outer = nlohmann::json::parse(json_);

  for (auto &json_inner : json_outer) {
    for (nlohmann::json::iterator it = json_inner.begin();
         it != json_inner.end();
         ++it) {
      const std::string &key = it.key();

      if (key == "text") text_ = it.value();
    }
  }
}

std::string docx_renderer_heading::Render() {
  if (!is_json_valid_markup_config_) throw "Failed render heading markup.\n";

  //wml_ = kWRunLhs;

  wml_ =
      "<w:p>"
        "<w:pPr>"
          "<w:pStyle w:val=\"para" + std::to_string(level_) + "\"/>"
        "</w:pPr>"
        "<w:r>"
          "<w:t>" + text_ + "</w:t>"
        "</w:r>"
      "</w:p>";

  //wml_ += kWRunRhs;

  return wml_;
}
