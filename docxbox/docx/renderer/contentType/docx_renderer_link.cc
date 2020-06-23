// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/docx/renderer/contentType/docx_renderer_link.h>

#include <iostream>

// Constructor
docx_renderer_link::docx_renderer_link(std::string path_extract,
                                       const std::string &json) {
  path_extract_ = std::move(path_extract);

  json_ = json;
  is_json_valid_ = InitFromJson();
}

bool docx_renderer_link::InitFromJson() {
  if (!helper::Json::IsJson(json_)
      || !docx_renderer::IsElementType(ElementType_Link)) return false;

  auto json_outer = nlohmann::json::parse(json_);

  for (auto &json_inner : json_outer) {
    for (nlohmann::json::iterator it = json_inner.begin();
         it != json_inner.end();
         ++it) {
      const std::string &key = it.key();
      auto &value = it.value();

      if (key == "text") text_ = value;
      else if (key == "url") url_ = value;
      else if (key == "pre" || key == "post") ExtractPreOrPostfix(it);
      // TODO(kay): add bookmark linking
//      else if (key == "bookmark")
    }
  }

  return !text_.empty() && !url_.empty();
}

void docx_renderer_link::SetRelationshipId(const std::string &relationship_id) {
  relationship_id_ = relationship_id;
}

std::string docx_renderer_link::Render(const std::string &relationship_id) {
  relationship_id_ = relationship_id;

  return Render();
}

std::string docx_renderer_link::Render() {
  if (!is_json_valid_) throw "Failed render list markup.\n";

  generic_root_tag_ = "w:p";

  wml_ =
      "<w:p>"
        "<w:pPr>"
          "<w:rPr>"
            "<w:rStyle w:val=\"char1\"/>"
          "</w:rPr>"
        "</w:pPr>"
        "<w:hyperlink r:id=\"" + relationship_id_ + "\" w:history=\"1\">"
          "<w:r>"
            "<w:rPr>"
              "<w:rStyle w:val=\"char1\"/>"
            "</w:rPr>"
            "<w:t>" + text_ + "</w:t>"
          "</w:r>"
        "</w:hyperlink>"
      "</w:p>";

  RenderPreAndPostFixAroundWml();

  return wml_;
}
