// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_DOCX_XML_COMPONENT_DOCX_XML_RELS_H_
#define DOCXBOX_DOCX_XML_COMPONENT_DOCX_XML_RELS_H_

#include <docxbox/docx/component/rels.h>
#include <docxbox/docx/xml/docx_xml.h>
#include <docxbox/docx/renderer/component/docx_renderer_rels.h>
#include <docxbox/helper/helper_file.h>
#include <docxbox/helper/helper_string.h>

#include <vendor/tinyxml2/tinyxml2.h>

#include <iostream>
#include <memory>
#include <string>
#include <utility>

class docx_xml_rels: docx_xml {
 public:
  explicit docx_xml_rels(
      std::string path_extract,
      int argc = 0,
      const std::vector<std::string>& argv = {});

  // Get (insert if not exists) relationship id of given image target
  std::string GetRelationShipIdByTarget(
      const std::string &target,
      rels::RelationType relation_type);

 private:
  std::string path_extract_;

  void GetRelationshipId(const tinyxml2::XMLElement *relationship,
                         std::string &relationship_id) const;
};

#endif  // DOCXBOX_DOCX_XML_COMPONENT_DOCX_XML_RELS_H_
