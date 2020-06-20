// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_DOCX_XML_VISITOR_DOCX_XML_TO_PLAINTEXT_H_
#define DOCXBOX_DOCX_XML_VISITOR_DOCX_XML_TO_PLAINTEXT_H_

#include <docxbox/docx/xml/docx_xml.h>
#include <docxbox/helper/helper_file.h>
#include <docxbox/helper/helper_string.h>

#include <vendor/tinyxml2/tinyxml2.h>

#include <string>
#include <iostream>

class docx_xml_to_plaintext:docx_xml {
 public:
  docx_xml_to_plaintext(int argc, const std::vector<std::string>& argv);

  std::string GetTextFromXmlFile(
      const std::string &path_xml,
      bool newline_at_segments = false);

  void GetChildNodesText(
      tinyxml2::XMLElement *node,
      bool newline_at_segments = false);

  void Output();
};

#endif  // DOCXBOX_DOCX_XML_VISITOR_DOCX_XML_TO_PLAINTEXT_H_
