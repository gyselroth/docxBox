// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/docx/xml/docx_xml_rels.h>

#include <utility>

docx_xml_rels::docx_xml_rels(
    std::string path_extract, int argc, char **argv) : docx_xml(
    argc,
    argv) {
  path_document_xml_rels_ = path_extract + "/word/_rels/document.xml.rels";
  path_extract_ = std::move(path_extract);

  xml_ = helper::File::GetFileContents(path_document_xml_rels_);
}

std::string docx_xml_rels::GetRelationShipIdByTarget(
  const std::string &path_target) {
  tinyxml2::XMLDocument doc;

  doc.Parse(xml_.c_str());

  if (doc.ErrorID() != 0) return "";

  tinyxml2::XMLElement *relationships = doc.FirstChildElement("Relationships");

  auto relationship = relationships->FirstChildElement();

  const char* target_key = "Target";

  bool target_exists = false;
  std::string relationship_id;

  do {
    auto target = relationship->FindAttribute(target_key);
    if (!target) continue;

    std::string target_value = target->Value();

    relationship_id =
        GetAttributeValueFromRelationship(relationship, relationship_id);

    if (target_value == path_target) {
      target_exists = true;
      break;
    }
  } while ((relationship = relationship->NextSiblingElement()));

  if (target_exists) return relationship_id;

  if (relationship_id.length() < 4) return "";

  auto last_id = std::stoi(relationship_id.substr(3));

  relationship_id = "rId" + std::to_string(last_id + 1);

  // TODO(kay): add Relationhship node

  return relationship_id;
}
std::string &docx_xml_rels::GetAttributeValueFromRelationship(const tinyxml2::XMLElement *relationship,
                                                              std::string &relationship_id) const {
  const char* id_key = "Id";
  auto id = relationship->FindAttribute(id_key);
  relationship_id = id->Value();
  return relationship_id;
}
