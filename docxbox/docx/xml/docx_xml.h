// Copyright (c) 2020 gyselroth GmbH

#ifndef DOCXBOX_DOCX_DOCX_XML_H_
#define DOCXBOX_DOCX_DOCX_XML_H_

#include <docxbox/helper/helper_file.h>
#include <docxbox/helper/helper_string.h>

#include <vendor/tinyxml2/tinyxml2.h>

#include <string>
#include <iostream>

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
