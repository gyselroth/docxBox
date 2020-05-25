// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/docx/renderer/component/docx_renderer_rels.h>

std::string docx_renderer_rels::RenderRelationship(
    const std::string &target,
    const int relation_type,
    const std::string& relationship_id) {
  switch (relation_type) {
    case rels::RelationType_Hyperlink:
      return RenderHyperlinkRelationship(target, relationship_id);
    case rels::RelationType_Numbering:
      return RenderNumberingRelationship(relationship_id);
    case rels::RelationType_Image:
    default:
      return RenderImageRelationship(target, relationship_id);
  }
}

std::string docx_renderer_rels::RenderHyperlinkRelationship(
    const std::string &url,
    const std::string &relationship_id) {
  return "<Relationship "
           "Id=\"" + relationship_id +"\" "
           "Type=\"" + kUrlRelationshipSchemaHyperlink + "\" "
           "Target=\"" + url + "\" "
           "TargetMode=\"External\"/>";
}

std::string docx_renderer_rels::RenderImageRelationship(
    const std::string &path_target,
    const std::string &relationship_id) {
  return "<Relationship "
         "Id=\"" + relationship_id + "\" "
         "Type=\"" + kUrlRelationshipSchemaImage + "\" "
         "Target=\"" + path_target + "\"/>";
}

std::string docx_renderer_rels::RenderNumberingRelationship(
    const std::string &relationship_id) {
  return "<Relationship "
           "Id=\"" + relationship_id + "\" "
           "Type=\"" + kUrlRelationshipSchemaNumbering + "\" "
           "Target=\"numbering.xml\"/>";
}
