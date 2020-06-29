// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/docx/component/rels.h>
#include <docxbox/docx/xml/component/docx_xml_rels.h>
#include <docxbox/docx/component/media.h>

const char rels::URL_SCHEMA_WORD_2010[] =
    "http://schemas.microsoft.com/office/word/2010/";

const char rels::URL_SCHEMA_OFFICE_2006[] =
    "http://schemas.openxmlformats.org/officeDocument/2006/";

const char rels::URL_SCHEMA_RELS_IMAGE[] =
    "http://schemas.openxmlformats.org/"
    "officeDocument/2006/relationships/image";

const char rels::URL_SCHEMA_RELS_HYPERLINK[] =
    "http://schemas.openxmlformats.org/"
    "officeDocument/2006/relationships/hyperlink";

const char rels::URL_SCHEMA_RELS_NUMBERING[] =
    "http://schemas.openxmlformats.org/"
    "officeDocument/2006/relationships/numbering";

rels::rels(int argc,
           std::vector<std::string> argv,
           std::string path_working_dir,
           std::string path_extract)
    : argc_(argc),
      argv_(std::move(argv)),
      path_working_dir_(std::move(path_working_dir)),
      path_extract_(std::move(path_extract)) {}

bool rels::HasAddedImageFile() const {
  return added_image_file_;
}

// Get (add if not exists) relationship into _rels/document.xml.rels
std::string rels::GetRelationshipId(std::string path_extract_absolute,
                                    const std::string &target,
                                    RelationType relation_type) {
  auto rels = new docx_xml_rels(std::move(path_extract_absolute));
  auto relationship_id = rels->GetRelationShipIdByTarget(target, relation_type);

  delete rels;

  return relationship_id;
}

bool rels::AddRelationsAndReferences(
    const std::string& replacement,
    std::string *image_relationship_id,
    std::string *hyperlink_relationship_id) {
  const char *kReplacement = replacement.c_str();

  auto render_type = docx_renderer::DetectElementType(kReplacement);

  if (render_type == docx_renderer::ElementType_Image) {
    *image_relationship_id = AddImageFileAndRelation(replacement);

    if ((*image_relationship_id).empty()) return false;
  } else if (render_type == docx_renderer::ElementType_Link) {
    *hyperlink_relationship_id = AddHyperlinkRelation(replacement);

    if ((*hyperlink_relationship_id).empty()) return false;
  } else if (render_type == docx_renderer::ElementType_ListOrdered
    /*|| render_type == docx_renderer::ElementType_ListUnordered*/  ) {
    std::string path_extract_absolute =
        path_working_dir_ + "/" + path_extract_;

    if (!contentTypes::AddOverrideNumberingReference(path_extract_absolute))
      return false;
  }

  return true;
}

/**
 * @param image_markup_json
 * @return relationship id, e.g. "rId7"
 */
std::string rels::AddImageFileAndRelation(
    const std::string &image_markup_json) {
  if (!docxbox::AppArgument::isArgImageFile(argc_, argv_, 5))
    // No media file given: successfully done (nothing)
    return "";

  std::string path_extract_absolute = path_extract_;
  helper::File::ResolvePath(path_working_dir_, &path_extract_absolute);

  auto relations = new media(path_extract_absolute);

  added_image_file_ = true;

  // 1. Copy image file to word/media/image<number>.<extension>
  std::string path_image = argv_[5];
  helper::File::ResolvePath(path_working_dir_, &path_image);

  if (!relations->AddImageFile(path_image)) {
    delete relations;

    docxbox::AppLog::NotifyError("Failed adding image file " + argv_[5]);

    added_image_file_ = false;
  }

  std::string relationship_id;

  if (added_image_file_) {
    // 2. Create media relation in _rels/document.xml.rels
    relationship_id = relations->GetImageRelationshipId(
        relations->GetMediaPathNewImage());
  }

  delete relations;

  return relationship_id;
}

std::string rels::AddHyperlinkRelation(
    const std::string &markup_json) {
  std::string path_extract_absolute = path_extract_;
  helper::File::ResolvePath(path_working_dir_, &path_extract_absolute);

  auto url = helper::Json::GetFirstValueOfKey(markup_json, "url");

  return GetRelationshipId(path_extract_absolute,
                           url,
                           rels::RelationType_Hyperlink);
}
