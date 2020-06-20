// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/docx/component/rels.h>
#include <docxbox/docx/xml/component/docx_xml_rels.h>

const std::string rels::URL_SCHEMA_WORD_2010 =
    "http://schemas.microsoft.com/office/word/2010/";

const std::string rels::URL_SCHEMA_OFFICE_2006 =
    "http://schemas.openxmlformats.org/officeDocument/2006/";

const std::string rels::URL_SCHEMA_RELS_IMAGE =
    "http://schemas.openxmlformats.org/officeDocument/2006/relationships/image";

const std::string rels::URL_SCHEMA_RELS_HYPERLINK =
    "http://schemas.openxmlformats.org/officeDocument/2006/relationships/"
    "hyperlink";

const std::string rels::URL_SCHEMA_RELS_NUMBERING =
    "http://schemas.openxmlformats.org/officeDocument/2006/relationships/"
    "numbering";

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
