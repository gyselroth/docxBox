// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_DOCX_XML_DOCX_XML_INDENT_H_
#define DOCXBOX_DOCX_XML_DOCX_XML_INDENT_H_

#include <docxbox/app/app_error.h>
#include <docxbox/docx/xml/docx_xml.h>
#include <docxbox/helper/helper_file.h>
#include <docxbox/helper/helper_string.h>

#include <vendor/tinyxml2/tinyxml2.h>

#include <iostream>
#include <string>
#include <vector>

class docx_xml_indent:docx_xml {
 public:
  docx_xml_indent(int argc, char **argv);

  static bool IndentXmlFile(const std::string& path_xml);

  static bool CompressXml(std::string &xml);
};

#endif  // DOCXBOX_DOCX_XML_DOCX_XML_INDENT_H_
