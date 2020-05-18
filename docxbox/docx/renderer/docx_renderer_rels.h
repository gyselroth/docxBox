// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_DOCX_RENDERER_DOCX_RENDERER_RELS_H_
#define DOCXBOX_DOCX_RENDERER_DOCX_RENDERER_RELS_H_

#include <docxbox/docx/renderer/docx_renderer.h>
#include <docxbox/docx/xml/docx_xml_rels.h>

#include <string>

static const char *const kUrlRelationshipSchemaImage =
  "http://schemas.openxmlformats.org/officeDocument/2006/relationships/image";

static const char *const kUrlRelationshipSchemaNumbering =
  "http://schemas.openxmlformats.org/officeDocument/2006/relationships/"
  "numbering";

class docx_renderer_rels: docx_renderer {
 public:
  static std::string RenderRelationship(
      const std::string &path_target,
      const int relation_type,
      const std::string& relationship_id);

  static std::string RenderImageRelationship(
      const std::string &path_target,
      const std::string &relationship_id);

  static std::string RenderNumberingRelationship(
      const std::string &relationship_id);
};

#endif  // DOCXBOX_DOCX_RENDERER_DOCX_RENDERER_RELS_H_
