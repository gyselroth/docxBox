// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/docx/renderer/docx_renderer.h>

const char docx_renderer::TAG_LHS_RUN[] = "<w:r>";
const char docx_renderer::TAG_RHS_RUN[] = "</w:r>";
const char docx_renderer::TAG_LHS_TABLE[] = "<w:tbl>";
const char docx_renderer::TAG_RHS_TABLE[] = "</w:tbl>";

docx_renderer::ElementType docx_renderer::DetectElementType(
    const std::string& json) {
  std::string identifier = helper::Json::GetFirstKey(json);

  if (identifier.empty()) return ElementType_None;
  if (identifier == "h1") return ElementType_Heading1;
  if (identifier == "h2") return ElementType_Heading2;
  if (identifier == "h3") return ElementType_Heading3;
  if (identifier == "image" || identifier == "img") return ElementType_Image;
  if (identifier == "link") return ElementType_Link;
  if (identifier == "ol") return ElementType_ListOrdered;
  if (identifier == "p") return ElementType_Paragraph;
  if (identifier == "table") return ElementType_Table;
  if (identifier == "text") return ElementType_Text;
  if (identifier == "ul") return ElementType_ListUnordered;

  return ElementType_None;
}

void docx_renderer::ExtractPreOrPostfix(const nlohmann::json::iterator& it) {
  const std::string &key = it.key();

  if (key == "post") {
    auto postfix_config = it.value();

    for (nlohmann::json::iterator it_post_config = postfix_config.begin();
         it_post_config != postfix_config.end();
         ++it_post_config) {
      const std::string& config_key = it_post_config.key();

      if (config_key == "text") {
        is_postfix_paragraph_ = false;
        postfix_text_ = it_post_config.value();
      } else if (config_key == "p" || config_key == "paragraph") {
        is_postfix_paragraph_ = true;
        postfix_text_ = it_post_config.value();
      }
    }
  } else {
    if (key != "pre") return;
  }

  auto prefix_config = it.value();

  for (nlohmann::json::iterator it_prefix_config = prefix_config.begin();
       it_prefix_config != prefix_config.end();
       ++it_prefix_config) {
    const std::string& config_key = it_prefix_config.key();

    if (config_key == "text") {
      is_prefix_paragraph_ = false;
      prefix_text_ = it_prefix_config.value();
    } else if (config_key == "p" || config_key == "paragraph") {
      is_postfix_paragraph_ = true;
      prefix_text_ = it_prefix_config.value();
    }
  }
}

void docx_renderer::RenderPreAndPostFixAroundWml() {
  bool has_prefix = false;
  bool has_postfix = false;

  // TODO(kay): change/implement pre/postfix rendering as "text"
  //            probably just needs leading/trailing w:t in same run

  if (!prefix_text_.empty()) {
    has_prefix = true;
    wml_ = RenderTextInRunInParagraph(prefix_text_);

//    wml_ = (is_prefix_paragraph_
//            ? RenderTextInRunInParagraph(prefix_text_)
//            : RenderTextInRun(prefix_text_)) + wml_;
  }

  if (!postfix_text_.empty()) {
    has_postfix = true;
    wml_ += RenderTextInRunInParagraph(postfix_text_);

//    wml_ += is_postfix_paragraph_
//            ? RenderTextInRunInParagraph(postfix_text_)
//            : RenderTextInRun(postfix_text_);
  }

  if (has_prefix || has_postfix) {
    // Wrap content and prefix/postfix in run to be injected than
    wml_ = "<w:p>" + wml_ + "</w:p>";

    generic_root_tag_ = "w:p";
  }
}

bool docx_renderer::IsElementType(const std::vector<ElementType>& types) {
  ElementType kElementType = DetectElementType(json_);

  for (auto type : types) if (kElementType == type) return true;

  return false;
}

bool docx_renderer::IsElementType(ElementType type) {
  return DetectElementType(json_) == type;
}

std::string docx_renderer::RenderTextInRun(std::string &text) {
  return "<w:r><w:t>" + text + "</w:t></w:r>";
}

std::string docx_renderer::RenderTextInRunInParagraph(std::string &text) {
  return "<w:p>" + RenderTextInRun(text) + "</w:p>";
}

int docx_renderer::PixelsToEmus(std::string &str) {
  std::string number = helper::String::ExtractLeadingNumber(str);

  return helper::String::IsNumeric(number)
    ? std::stoi(number) * 9525
    : 0;
}
