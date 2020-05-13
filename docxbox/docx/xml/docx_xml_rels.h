// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_DOCX_XML_DOCX_XML_RELS_H_
#define DOCXBOX_DOCX_XML_DOCX_XML_RELS_H_

#include <docxbox/docx/xml/docx_xml.h>
#include <docxbox/helper/helper_file.h>
#include <docxbox/helper/helper_string.h>

#include <vendor/tinyxml2/tinyxml2.h>

#include <string>
#include <iostream>

class docx_xml_rels:docx_xml {
 public:
  docx_xml_rels(std::string path_extract, int argc = 0, char **argv = nullptr);

  std::string GetRelationShipIdByTarget(const std::string &path_target);

 private:
  std::string path_extract_;
};

#endif  // DOCXBOX_DOCX_XML_DOCX_XML_RELS_H_
