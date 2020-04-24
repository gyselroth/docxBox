// Copyright (c) 2020 gyselroth GmbH

#include <docxbox/docx/renderer/docx_renderer.h>

docx_renderer::Elements docx_renderer::DetectElementType(
    const std::string& json) {
  int offset_end_identifier = helper::String::OffsetChar(json, '"', 2) - 2;

  if (offset_end_identifier == -1) return Element_None;

  std::string identifier = json.substr(2, offset_end_identifier);

  if (identifier == "h1") return Element_Header1;
  if (identifier == "h2") return Element_Header2;
  if (identifier == "h3") return Element_Header3;
  if (identifier == "h4") return Element_Header4;
  if (identifier == "link") return Element_Link;
  if (identifier == "img") return Element_Image;
  if (identifier == "ol") return Element_OrderedList;
  if (identifier == "table") return Element_Table;
  if (identifier == "p") return Element_Paragraph;
  if (identifier == "ul") return Element_UnorderedList;

  return Element_None;
}
