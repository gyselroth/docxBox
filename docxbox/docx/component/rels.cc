// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/docx/component/rels.h>
#include <docxbox/docx/xml/component/docx_xml_rels.h>

// Get (add if not exists) relationship into _rels/document.xml.rels
std::string rels::GetRelationshipId(
    std::string path_extract_absolute,
    const std::string &target,
    RelationType relation_type) {
  auto rels = new docx_xml_rels(std::move(path_extract_absolute));
  auto relationship_id = rels->GetRelationShipIdByTarget(target, relation_type);

  delete rels;

  return relationship_id;
}
