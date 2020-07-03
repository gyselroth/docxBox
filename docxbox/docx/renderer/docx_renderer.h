// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_DOCX_RENDERER_DOCX_RENDERER_H_
#define DOCXBOX_DOCX_RENDERER_DOCX_RENDERER_H_

#include <docxbox/app/app_log.h>
#include <docxbox/helper/helper_string.h>
#include <docxbox/helper/helper_json.h>

#include <iostream>
#include <string>
#include <utility>
#include <vector>

class docx_renderer {
 public:
  enum ElementType {
    ElementType_Heading1,
    ElementType_Heading2,
    ElementType_Heading3,
    ElementType_Image,
    ElementType_Link,
    ElementType_ListUnordered,
    ElementType_Paragraph,
    ElementType_Table,
    ElementType_Text,
    ElementType_None
  };

  static const char TAG_LHS_RUN[];
  static const char TAG_RHS_RUN[];
  static const char TAG_LHS_TABLE[];
  static const char TAG_RHS_TABLE[];

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

  static std::string RenderTextInRun(const std::string &text);
  static std::string RenderTextInRunInParagraph(const std::string &text);

  static int PixelsToEmus(const std::string &str);

 private:
  virtual bool InitFromJson() = 0;
};

#endif  // DOCXBOX_DOCX_RENDERER_DOCX_RENDERER_H_
