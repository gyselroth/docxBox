// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_DOCX_DOCX_STYLES_H_
#define DOCXBOX_DOCX_DOCX_STYLES_H_

#include <string>
#include <utility>

class docx_styles {
 public:
  enum StyleType {
    StyleType_Character,
    StyleType_Table,
    StyleType_Paragraph
  };

  enum StyleName {
    StyleName_Hyperlink,
    StyleName_Table,
    StyleName_Paragraph
  };

  // Insert style if not existing yet
  static std::string AddStyle(
      std::string path_extract_absolute,
      StyleType type,
      StyleName name);
};

#endif  // DOCXBOX_DOCX_DOCX_STYLES_H_
