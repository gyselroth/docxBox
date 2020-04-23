// Copyright (c) 2020 gyselroth GmbH

#ifndef DOCXBOX_DOCX_WML_RENDERER_DOCX_WML_RENDERER_H_
#define DOCXBOX_DOCX_WML_RENDERER_DOCX_WML_RENDERER_H_

#include <docxbox/helper/helper_string.h>

#include <iostream>
#include <string>

static const char *const kWRunLhs = "<w:r>";
static const char *const kWRunRhs = "</w:r>";

static const char *const kWTableLhs = "<w:tbl>";
static const char *const kWTableRhs = "</w:tbl>";

class docx_wml_renderer {
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

  static docx_wml_renderer::Elements DetectElementType(const std::string& json);

 protected:
  std::string json_;

  bool is_valid_wml_json_;

  std::string wml_;
};

#endif  // DOCXBOX_DOCX_WML_RENDERER_DOCX_WML_RENDERER_H_
