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

  auto relationship = relationships->FirstChild();

  do {
    // TODO(kay): find given path_target in Target attribute of all nodes
  } while ((relationship = relationship->NextSiblingElement()));

  // TODO(kay): if path_target found: return its value of node's Id attribute

  // TODO(kay): if path_target NOT found: add new Relationship node
  //            w/ Id = (highest existing Id) + 1

  // TODO(kay): return Id

  return "";
}
