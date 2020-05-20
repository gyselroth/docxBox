// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/docx/renderer/docx_renderer_styles.h>

std::string docx_renderer_styles::RenderStyle(
    const int style_type,
    const std::string& style_id) {
  switch (style_type) {
    case docx_rels::RelationType_Hyperlink:
    default:
      return RenderHyperlinkStyle(style_id);
  }
}

std::string docx_renderer_styles::RenderHyperlinkStyle(
    const std::string &style_id) {
  return
    "<w:style w:type=\"character\" w:styleId=\"" + style_id + "\">"
      "<w:name w:val=\"Hyperlink\"/>"
      "<w:rPr>"
        "<w:color w:val=\"0000ff\"/>"
        "<w:u w:color=\"auto\" w:val=\"single\"/>"
      "</w:rPr>"
   "</w:style>";
}
