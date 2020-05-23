// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_DOCX_XML_ENTITY_DOCX_XML_STYLES_H_
#define DOCXBOX_DOCX_XML_ENTITY_DOCX_XML_STYLES_H_

#include <docxbox/docx/component/styles.h>
#include <docxbox/docx/xml/docx_xml.h>
#include <docxbox/docx/renderer/docx_renderer_styles.h>
#include <docxbox/docx/xml/visitor/docx_xml_indent.h>
#include <docxbox/helper/helper_file.h>
#include <docxbox/helper/helper_string.h>

#include <vendor/tinyxml2/tinyxml2.h>

#include <iostream>
#include <string>
#include <utility>


class docx_xml_styles: docx_xml {
 public:
  explicit docx_xml_styles(
      std::string path_extract, int argc = 0, char **argv = nullptr);

  // Get (insert if not exists) style ID
  std::string GetStyleId(
      styles::StyleType type, styles::StyleName name);

 private:
  std::string path_extract_;
  std::string path_styles_xml;

  std::string xml_;

  bool SaveXml(bool compress = false);

  static std::string GetStyleIdPrefixByType(styles::StyleType type);
};

#endif  // DOCXBOX_DOCX_XML_ENTITY_DOCX_XML_STYLES_H_
