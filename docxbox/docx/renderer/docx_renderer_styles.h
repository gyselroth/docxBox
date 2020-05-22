// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_DOCX_RENDERER_DOCX_RENDERER_STYLES_H_
#define DOCXBOX_DOCX_RENDERER_DOCX_RENDERER_STYLES_H_

#include <docxbox/docx/renderer/docx_renderer.h>
#include <docxbox/docx/xml/docx_xml_rels.h>

#include <string>

class docx_renderer_styles: docx_renderer {
 public:
  static std::string RenderStyle(
      const int style_type, const std::string& style_id);

  static std::string RenderHyperlinkStyle(const std::string &style_id);
};

#endif  // DOCXBOX_DOCX_RENDERER_DOCX_RENDERER_RELS_H_
