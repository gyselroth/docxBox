// Copyright (c) 2020 gyselroth GmbH

#ifndef DOCXBOX_DOCX_DOCX_XML_H_
#define DOCXBOX_DOCX_DOCX_XML_H_

#include <string>
#include <iostream>

#include "../helper/helper_file.h"
#include "../helper/helper_string.h"

#include "../../vendor/tinyxml2/tinyxml2.h"

class docx_xml {
 public:
  docx_xml(int argc, char **argv);

  static bool IsXmlFileContainingText(const std::string &filename);

 protected:
  int argc_;
  char **argv_;

  std::string document_text_;
};

#endif  // DOCXBOX_DOCX_DOCX_XML_H_
