// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/docx/renderer/docx_renderer.h>

docx_renderer::Elements docx_renderer::DetectElementType(
    const std::string& json) {
  int offset_end_identifier = helper::String::OffsetChar(json, '"', 3);

  if (offset_end_identifier == -1) return Element_None;

  std::string identifier = json.substr(2, offset_end_identifier - 2);

  if (identifier == "h1") return Element_Header1;
  if (identifier == "h2") return Element_Header2;
  if (identifier == "h3") return Element_Header3;
  if (identifier == "h4") return Element_Header4;
  if (identifier == "link") return Element_Link;
  if (identifier == "image") return Element_Image;
  if (identifier == "img") return Element_Image;
  if (identifier == "ol") return Element_OrderedList;
  if (identifier == "table") return Element_Table;
  if (identifier == "p") return Element_Paragraph;
  if (identifier == "ul") return Element_UnorderedList;

  return Element_None;
}

bool docx_renderer::IsJsonForImage(const std::string &str) {
  return helper::String::IsJson(str)
      && docx_renderer::Element_Image ==
          docx_renderer::DetectElementType(str);
}