// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/docx/renderer/docx_renderer.h>

docx_renderer::ElementType docx_renderer::DetectElementType(
    const std::string& json) {
  int offset_end_identifier = helper::String::OffsetChar(json, '"', 3);

  if (offset_end_identifier == -1) return ElementType_None;

  std::string identifier = json.substr(2, offset_end_identifier - 2);

  if (identifier == "h1") return ElementType_Heading1;
  if (identifier == "h2") return ElementType_Heading2;
  if (identifier == "h3") return ElementType_Heading3;
  if (identifier == "link") return ElementType_Link;
  if (identifier == "image") return ElementType_Image;
  if (identifier == "img") return ElementType_Image;
  if (identifier == "ol") return ElementType_ListOrdered;
  if (identifier == "table") return ElementType_Table;
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
