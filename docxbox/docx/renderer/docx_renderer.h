// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_DOCX_RENDERER_DOCX_RENDERER_H_
#define DOCXBOX_DOCX_RENDERER_DOCX_RENDERER_H_

#include <docxbox/helper/helper_string.h>

#include <iostream>
#include <string>

static const char *const kWRunLhs = "<w:r>";
static const char *const kWRunRhs = "</w:r>";

static const char *const kWTableLhs = "<w:tbl>";
static const char *const kWTableRhs = "</w:tbl>";

class docx_renderer {
 public:
  enum Elements {
    Element_Header1,
    Element_Header2,
    Element_Header3,
    Element_Header4,
    Element_Link,
    Element_Image,
    Element_OrderedList,
    Element_Table,
    Element_Paragraph,
    Element_UnorderedList,
    Element_None
  };

  static docx_renderer::Elements DetectElementType(const std::string& json);

 protected:
  int argc_;
  char **argv_;

  std::string json_;

  bool is_json_valid_markup_config_;

  std::string wml_;
};

#endif  // DOCXBOX_DOCX_RENDERER_DOCX_RENDERER_H_
