// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/docx/renderer/contentType/docx_renderer_heading.h>

#include <iostream>
#include <utility>

// Constructor
docx_renderer_heading::docx_renderer_heading(std::string path_extract,
                                             const std::string &json) {
  path_extract_ = std::move(path_extract);

  json_ = json;
  is_json_valid_ = InitFromJson();

  level_ = 1;
}

void docx_renderer_heading::SetLevel(int level) {
  level_ = level;
}

bool docx_renderer_heading::InitFromJson() {
  if (!helper::Json::IsJson(json_)
      || !docx_renderer::IsElementType(
      {ElementType_Heading1, ElementType_Heading2, ElementType_Heading3}))
    return false;

  text_ = "";

  auto json_outer = nlohmann::json::parse(json_);

  for (auto &json_inner : json_outer) {
    for (nlohmann::json::iterator it = json_inner.begin();
         it != json_inner.end();
         ++it) {
      const std::string &key = it.key();
      const std::string &value = it.value();

      if (key == "text") text_ = value;
      else if (key == "pre" || key == "post") ExtractPreOrPostfix(it);
    }
  }

  return !text_.empty();
}

std::string docx_renderer_heading::Render() {
  return Render(1);
}

std::string docx_renderer_heading::Render(int level) {
  if (!is_json_valid_) throw "Failed render heading markup.\n";

  if (level > 0) SetLevel(level);

  generic_root_tag_ = "w:p";

  wml_ =
      "<w:p>"
        "<w:pPr>"
          "<w:pStyle w:val=\"para" + std::to_string(level_) + "\"/>"
        "</w:pPr>"
        + RenderTextInRun(text_)
    + "</w:p>";

  RenderPreAndPostFixAroundWml();

  return wml_;
}
