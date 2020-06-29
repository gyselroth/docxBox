// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_DOCX_COMPONENT_RELS_H_
#define DOCXBOX_DOCX_COMPONENT_RELS_H_

#include <docxbox/app/app_argument.h>
#include <docxbox/docx/component/contentTypes.h>

#include <string>
#include <utility>
#include <vector>

class rels {
 public:
  enum RelationType {
    RelationType_Hyperlink,
    RelationType_Image,
    RelationType_Numbering,
    RelationType_None
  };

  static const char URL_SCHEMA_WORD_2010[];
  static const char URL_SCHEMA_OFFICE_2006[];
  static const char URL_SCHEMA_RELS_IMAGE[];
  static const char URL_SCHEMA_RELS_HYPERLINK[];
  static const char URL_SCHEMA_RELS_NUMBERING[];

  rels(int argc,
       std::vector<std::string> argv,
       std::string path_working_dir,
       std::string path_extract);

  bool HasAddedImageFile() const;

  // Get relationship ID, insert if not existing yet
  static std::string GetRelationshipId(std::string path_extract_absolute,
                                       const std::string &target,
                                       RelationType relation_type);

  bool AddRelationsAndReferences(
      const std::string& replacement,
      std::string *image_relationship_id,
      std::string *hyperlink_relationship_id);

 private:
  int argc_;
  std::vector<std::string> argv_;

  std::string path_working_dir_;
  std::string path_extract_;

  // New image file added, requires adding resp. relation
  bool added_image_file_ = false;

  std::string AddImageFileAndRelation(const std::string &image_markup_json);
  std::string AddHyperlinkRelation(const std::string &markup_json);
};

#endif  // DOCXBOX_DOCX_COMPONENT_RELS_H_
