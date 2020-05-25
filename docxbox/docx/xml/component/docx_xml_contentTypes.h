// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_DOCX_XML_COMPONENT_DOCX_XML_CONTENTTYPES_H_
#define DOCXBOX_DOCX_XML_COMPONENT_DOCX_XML_CONTENTTYPES_H_

#include <docxbox/docx/xml/docx_xml.h>
#include <docxbox/helper/helper_file.h>
#include <docxbox/helper/helper_string.h>

#include <vendor/tinyxml2/tinyxml2.h>

#include <iostream>
#include <string>
#include <utility>

class docx_xml_contentTypes: docx_xml {
 public:
  explicit docx_xml_contentTypes(
      std::string path_extract, int argc = 0, char **argv = nullptr);

  bool OverrideNumbering();

 private:
  std::string path_extract_;
};

#endif  // DOCXBOX_DOCX_XML_COMPONENT_DOCX_XML_CONTENTTYPES_H_
