// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/docx/renderer/docx_renderer.h>

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
  if (identifier == "para" || identifier == "paragraph") return ElementType_Paragraph;
  if (identifier == "table") return ElementType_Table;
  if (identifier == "text") return ElementType_Text;
  if (identifier == "ul") return ElementType_ListUnordered;

  return ElementType_None;
}

bool docx_renderer::IsElementType(const std::vector<ElementType>& types) {
  ElementType kElementType = DetectElementType(json_);

  for (auto type : types) if (kElementType == type) return true;

  return false;
}

bool docx_renderer::IsElementType(ElementType type) {
  return DetectElementType(json_) == type;
}
