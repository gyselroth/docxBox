// Copyright (c) 2020 gyselroth GmbH

#ifndef SRC_DOCXBOX_DOCX_XML_FIELDS_H
#define SRC_DOCXBOX_DOCX_XML_FIELDS_H

#include <string>
#include <iostream>

#include "docx_xml.h"
#include "../helper/helper_file.h"

#include "../helper/helper_string.h"
#include "../../vendor/tinyxml2/tinyxml2.h"

class docx_xml_fields:docx_xml {
 public:
  docx_xml_fields(int argc, char **argv);

  void CollectMergeFields(std::string path_xml, bool as_json = false);

  void Output();

 private:
  bool as_json_;
  std::string fields_;

  static void CollectFieldsFromNodes(tinyxml2::XMLElement *node);
};

#endif  // SRC_DOCXBOX_DOCX_XML_FIELDS_H_
