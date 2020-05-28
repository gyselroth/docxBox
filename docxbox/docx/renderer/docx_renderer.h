// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_DOCX_RENDERER_DOCX_RENDERER_H_
#define DOCXBOX_DOCX_RENDERER_DOCX_RENDERER_H_

#include <docxbox/helper/helper_json.h>

#include <iostream>
#include <string>
#include <vector>

static const char *const kWRunLhs = "<w:r>";
static const char *const kWRunRhs = "</w:r>";

static const char *const kWTableLhs = "<w:tbl>";
static const char *const kWTableRhs = "</w:tbl>";

class docx_renderer {
 public:
  enum ElementType {
    ElementType_Heading1,
    ElementType_Heading2,
    ElementType_Heading3,
    ElementType_Image,
    ElementType_Link,
    ElementType_ListOrdered,
    ElementType_ListUnordered,
    ElementType_Paragraph,
    ElementType_Table,
    ElementType_Text,
    ElementType_None
  };

  // Root tag of rendered XML
  std::string generic_root_tag_;

  static docx_renderer::ElementType DetectElementType(const std::string& json);

  bool IsElementType(const std::vector<ElementType>& types);
  bool IsElementType(ElementType type);

  virtual std::string Render() = 0;

 protected:
  std::string path_extract_;

  std::string json_;
  bool is_json_valid_;

  // Temporary markers for batch templating
  std::string prefix_text_;
  std::string postfix_text_;
  // Pre- and postfix can be: paragraph or just text
  bool is_prefix_paragraph_ = false;
  bool is_postfix_paragraph_ = false;

  std::string wml_;

  void ExtractPreOrPostfix(const nlohmann::json::iterator& it);
  void RenderPreAndPostFixAroundWml();

  static std::string RenderTextInRun(std::string &text);
  static std::string RenderTextInRunInParagraph(std::string &text);

 private:
  virtual bool InitFromJson() = 0;
};

#endif  // DOCXBOX_DOCX_RENDERER_DOCX_RENDERER_H_
