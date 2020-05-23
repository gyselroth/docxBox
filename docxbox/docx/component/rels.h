// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_DOCX_COMPONENT_RELS_H_
#define DOCXBOX_DOCX_COMPONENT_RELS_H_

static const char *const kUrlSchemaMsOfficeWord2010 =
    "http://schemas.microsoft.com/office/word/2010/";

static const char *const kUrlSchemaOffice2006 =
    "http://schemas.openxmlformats.org/officeDocument/2006/";

static const char *const kUrlRelationshipSchemaImage =
    "http://schemas.openxmlformats.org/officeDocument/2006/relationships/image";

static const char *const kUrlRelationshipSchemaHyperlink =
    "http://schemas.openxmlformats.org/officeDocument/2006/relationships/"
    "hyperlink";

static const char *const kUrlRelationshipSchemaNumbering =
    "http://schemas.openxmlformats.org/officeDocument/2006/relationships/"
    "numbering";

#include <string>
#include <utility>

class rels {
 public:
  enum RelationType {
    RelationType_Hyperlink,
    RelationType_Image,
    RelationType_Numbering,
    RelationType_None
  };

  // Get relationship id, insert if not existing yet
  static std::string GetRelationshipId(
      std::string path_extract_absolute,
      const std::string &target,
      RelationType relation_type);
};

#endif  // DOCXBOX_DOCX_COMPONENT_RELS_H_
