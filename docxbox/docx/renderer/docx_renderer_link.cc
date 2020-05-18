// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/docx/renderer/docx_renderer_link.h>

#include <iostream>

// Constructor
docx_renderer_link::docx_renderer_link(
    int argc, char **argv, const std::string &json) {
  argc_ = argc;
  argv_ = argv;

  json_ = json;
  is_json_valid_markup_config_ = helper::String::IsJson(json);

  if (is_json_valid_markup_config_) InitSpecsFromJson();
}

void docx_renderer_link::InitSpecsFromJson() {
//  if (!docx_renderer::IsValidJsonForLink(json_)) {
//    is_json_valid_markup_config_ = false;
//
//    return;
//  }

  auto json_outer = nlohmann::json::parse(json_);

  for (auto &json_inner : json_outer) {
    for (nlohmann::json::iterator it = json_inner.begin();
         it != json_inner.end();
         ++it) {
      const std::string &key = it.key();

      if (key == "text") text_ = it.value();
      else if (key == "url") url_ = it.value();
      // TODO(kay): add bookmark linking
      //else if (key == "bookmark")
    }
  }
}

std::string docx_renderer_link::Render() {
  if (!is_json_valid_markup_config_) throw "Failed render list markup.\n";

  wml_ =
      "<w:p>"
        "<w:pPr>"
          "<w:rPr>"
            "<w:rStyle w:val=\"char1\"/>"
          "</w:rPr>"
        "</w:pPr>"
        // TODO(kay): add link relation into word/_rels/document.xml.rels
        "<w:hyperlink r:id=\"rId7\" w:history=\"1\">"
          "<w:r>"
            "<w:rPr>"
              "<w:rStyle w:val=\"char1\"/>"
            "</w:rPr>"
            "<w:t>" + text_ + "</w:t>"
          "</w:r>"
        "</w:hyperlink>"
      "</w:p>";

  return wml_;
}
