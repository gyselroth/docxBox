// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/docx/renderer/docx_renderer_rels.h>

std::string docx_renderer_rels::RenderImageRelationship(
    const std::string &path_target,
    const std::string &relationship_id) {
  return "<Relationship "
          "Id=\"" + relationship_id + "\" "
          "Type=\"" + kUrlRelationshipSchemaImage + "\" "
          "Target=\"" + path_target + "\"/>";
}
