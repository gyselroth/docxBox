// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/docx/docx_styles.h>
#include <docxbox/docx/xml/docx_xml_styles.h>

// Insert style if not existing yet
std::string docx_styles::AddStyle(
    std::string path_extract_absolute, StyleType type, StyleName name) {
  auto styles = new docx_xml_styles(std::move(path_extract_absolute));
  auto style_id = styles->GetStyleId(type, name);

  delete styles;

  return style_id;
}
