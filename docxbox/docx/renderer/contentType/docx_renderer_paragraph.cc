// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/docx/renderer/contentType/docx_renderer_paragraph.h>
#include <docxbox/app/app_log.h>

// Constructor
docx_renderer_paragraph::docx_renderer_paragraph(std::string path_extract,
                                                 const std::string &json) {
  path_extract_ = std::move(path_extract);

  json_ = json;
  is_json_valid_ = InitFromJson();
}

bool docx_renderer_paragraph::InitFromJson() {
  if (!helper::Json::IsJson(json_)
      || !docx_renderer::IsElementType(ElementType_Paragraph))
    return false;

  text_ = "";

  auto json_outer = nlohmann::json::parse(json_);

  for (auto &json_inner : json_outer) {
    for (nlohmann::json::iterator it = json_inner.begin();
         it != json_inner.end();
         ++it) {
      const std::string &key = it.key();

      if (key == "text") text_ = it.value();
      else if (key == "pre" || key == "post") ExtractPreOrPostfix(it);
    }
  }

  return !text_.empty();
}

std::string docx_renderer_paragraph::Render() {
  if (!is_json_valid_) {
    docxbox::AppLog::NotifyError("Failed render text markup");

    return "";
  }

  generic_root_tag_ = "w:p";

  wml_ = RenderTextInRunInParagraph(text_);

  RenderPreAndPostFixAroundWml();

  return wml_;
}
