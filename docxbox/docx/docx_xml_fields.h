// Copyright (c) 2020 gyselroth GmbH

#ifndef DOCXBOX_DOCX_DOCX_XML_FIELDS_H_
#define DOCXBOX_DOCX_DOCX_XML_FIELDS_H_

#include <string>
#include <iostream>

#include "docx_xml.h"
#include "../helper/helper_file.h"

#include "../helper/helper_string.h"
#include "../../vendor/tinyxml2/tinyxml2.h"

class docx_xml_fields:docx_xml {
 public:
  docx_xml_fields(int argc, char **argv);

  void CollectMergeFields(const std::string& path_xml, bool as_json = false);

  void Output(bool as_json);
 private:
  std::vector <std::string> fields_in_current_xml_;

  // Vector of XML filenames that fields have been collected from
  std::vector <std::string> field_xml_files_;
  // Vector of vectors: fields per XML file
  std::vector <std::vector<std::string>> fields_in_xml_files_;

  void CollectFieldsFromNodes(tinyxml2::XMLElement *node);
  void OutputAsJson();
};

#endif  // DOCXBOX_DOCX_DOCX_XML_FIELDS_H_
