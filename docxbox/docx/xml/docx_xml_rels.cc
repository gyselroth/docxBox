// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/docx/xml/docx_xml_rels.h>

#include <utility>

docx_xml_rels::docx_xml_rels(
    std::string path_extract, int argc, char **argv) : docx_xml(
    argc,
    argv) {
  path_extract_ = std::move(path_extract);
}

std::string docx_xml_rels::GetRelationShipIdByTarget(
  const std::string &path_target) {

  return "";

//  tinyxml2::XMLDocument doc;
//
//  doc.LoadFile(path_xml.c_str());
//
//  if (doc.ErrorID() != 0) return "";
//
//  tinyxml2::XMLElement *body = doc.FirstChildElement("w:document")
//      ->FirstChildElement("w:body");
//
//  GetChildNodesText(body, newline_at_segments);
//
//  return document_text_;
}
