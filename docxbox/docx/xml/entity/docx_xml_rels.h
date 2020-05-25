// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_DOCX_XML_ENTITY_DOCX_XML_RELS_H_
#define DOCXBOX_DOCX_XML_ENTITY_DOCX_XML_RELS_H_

#include <docxbox/docx/component/rels.h>
#include <docxbox/docx/xml/docx_xml.h>
#include <docxbox/docx/renderer/docx_renderer_rels.h>
#include <docxbox/docx/xml/visitor/docx_xml_indent.h>
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
      std::string path_extract, int argc = 0, char **argv = nullptr);

  // Get (insert if not exists) relationship id of given image target
  std::string GetRelationShipIdByTarget(
      const std::string &target,
      rels::RelationType relation_type);

 private:
  std::string path_extract_;
  std::string path_rels_xml;

  std::string xml_;

  void GetRelationshipId(const tinyxml2::XMLElement *relationship,
                         std::string &relationship_id) const;

  bool SaveXml(bool compress = false);
};

#endif  // DOCXBOX_DOCX_XML_ENTITY_DOCX_XML_RELS_H_
