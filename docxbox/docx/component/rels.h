// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_DOCX_COMPONENT_RELS_H_
#define DOCXBOX_DOCX_COMPONENT_RELS_H_

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

  static const std::string URL_SCHEMA_WORD_2010;
  static const std::string URL_SCHEMA_OFFICE_2006;
  static const std::string URL_SCHEMA_RELS_IMAGE;
  static const std::string URL_SCHEMA_RELS_HYPERLINK;
  static const std::string URL_SCHEMA_RELS_NUMBERING;

  // Get relationship id, insert if not existing yet
  static std::string GetRelationshipId(
      std::string path_extract_absolute,
      const std::string &target,
      RelationType relation_type);
};

#endif  // DOCXBOX_DOCX_COMPONENT_RELS_H_
