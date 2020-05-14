// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_DOCX_RENDERER_DOCX_RENDERER_RELS_H_
#define DOCXBOX_DOCX_RENDERER_DOCX_RENDERER_RELS_H_

#include <docxbox/docx/renderer/docx_renderer.h>

#include <string>

static const char *const kUrlRelationshipSchemaImage =
  "http://schemas.openxmlformats.org/officeDocument/2006/relationships/image";

class docx_renderer_rels: docx_renderer {
 public:
  static std::string RenderImageRelationship(
      const std::string &path_target,
      const std::string &relationship_id);
};

#endif  // DOCXBOX_DOCX_RENDERER_DOCX_RENDERER_RELS_H_
