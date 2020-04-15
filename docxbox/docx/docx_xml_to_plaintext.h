// Copyright (c) 2020 gyselroth GmbH

#ifndef DOCXBOX_DOCX_DOCX_XML_TO_PLAINTEXT_H_
#define DOCXBOX_DOCX_DOCX_XML_TO_PLAINTEXT_H_

#include <string>
#include <iostream>

#include "./docx_xml.h"
#include "../helper/helper_file.h"

#include "../helper/helper_string.h"
#include "../../vendor/tinyxml2/tinyxml2.h"

class docx_xml_to_plaintext:docx_xml {
 public:
  docx_xml_to_plaintext(int argc, char **argv);

  std::string GetTextFromXmlFile(
      const std::string &path_xml,
      bool newline_at_segments = false);

  void GetChildNodesText(
      tinyxml2::XMLElement *node,
      bool newline_at_segments = false);

  void Output();
};

#endif  // DOCXBOX_DOCX_DOCX_XML_TO_PLAINTEXT_H_
