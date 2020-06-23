// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_DOCX_RENDERER_CONTENTTYPE_DOCX_RENDERER_PARAGRAPH_H_
#define DOCXBOX_DOCX_RENDERER_CONTENTTYPE_DOCX_RENDERER_PARAGRAPH_H_

#include <docxbox/docx/renderer/docx_renderer.h>
#include <docxbox/helper/helper_string.h>

#include <string>
#include <utility>
#include <vector>

#include <vendor/json/single_include/nlohmann/json.hpp>

class docx_renderer_paragraph: public docx_renderer {
 public:
  // Constructor
  explicit docx_renderer_paragraph(std::string path_extract,
                                   const std::string &json);

  std::string Render() override;

 private:
  std::string text_;

  bool InitFromJson() override;
};

#endif  // DOCXBOX_DOCX_RENDERER_CONTENTTYPE_DOCX_RENDERER_PARAGRAPH_H_
