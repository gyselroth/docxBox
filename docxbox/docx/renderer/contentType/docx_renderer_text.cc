// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/docx/renderer/contentType/docx_renderer_text.h>

// Constructor
docx_renderer_text::docx_renderer_text(std::string path_extract,
                                       const std::string &json) {
  path_extract_ = std::move(path_extract);

  json_ = json;
  is_json_valid_ = InitFromJson();
}

bool docx_renderer_text::InitFromJson() {
  if (!docx_renderer::IsElementType(ElementType_Text))
    return false;

  text_ = "";

  try {
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
  } catch (nlohmann::detail::parse_error &e) {
    return docxbox::AppLog::NotifyError("Parse error - Invalid JSON: " + json_);
  }
}

std::string docx_renderer_text::Render() {
  if (!is_json_valid_) {
    docxbox::AppLog::NotifyError("Failed render text markup");

    return "";
  }

  generic_root_tag_ = "w:r";

  wml_ = RenderTextInRun(text_);

  RenderPreAndPostFixAroundWml();

  return wml_;
}
