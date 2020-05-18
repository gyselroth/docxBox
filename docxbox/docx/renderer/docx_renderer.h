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
    Element_Heading1,
    Element_Heading2,
    Element_Heading3,
    Element_Link,
    Element_ListOrdered,
    Element_ListUnordered,
    Element_Image,
    Element_Table,
    Element_None
  };

  static docx_renderer::Elements DetectElementType(const std::string& json);

  // TODO(kay): make these validator unnecessary: initFromJson should suffice as
  //            both- initialization and check: all required fields initialized?
  static bool IsValidJsonForHeading(const std::string &str);
  static bool IsValidJsonForImage(const std::string &str);
  static bool IsValidJsonForList(const std::string &str);

 protected:
  int argc_;
  char **argv_;

  std::string json_;

  bool is_json_valid_markup_config_;

  std::string wml_;
};

#endif  // DOCXBOX_DOCX_RENDERER_DOCX_RENDERER_H_
