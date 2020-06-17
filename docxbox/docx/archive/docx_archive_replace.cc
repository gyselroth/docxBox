// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/docx/archive/docx_archive_replace.h>
#include <docxbox/docx/component/contentTypes.h>

docx_archive_replace::docx_archive_replace(
    int argc,
    char **argv,
    bool is_batch_mode) : docx_archive(argc, argv, is_batch_mode) {}

bool docx_archive_replace::ReplaceImage() {
  if (!docxbox::AppArgument::AreArgumentsGiven(
      argc_,
      2, "DOCX filename",
      3, "Filename of image to be replaced",
      4, "Filename of replacement image")) return false;

  if (!is_batch_mode_
      && !UnzipDocxByArgv(true, "-" + helper::File::GetTmpName())) return false;

  std::string image_original = argv_[3];

  miniz_cpp::zip_file docx_file(path_docx_in_);

  auto file_list = docx_file.infolist();

  try {
    bool found = false;

    for (const auto &file_in_zip : file_list) {
      if (!helper::String::EndsWith(file_in_zip.filename, image_original))
        continue;

      found = true;

      std::string path_image_original =
          path_extract_ + "/" + file_in_zip.filename;

      if (!helper::File::Remove(path_image_original.c_str()))
        return docxbox::AppLog::NotifyError("Failed replace " + image_original);

      // TODO(kay): add adaptation for batch process-
      //            needs use of dynamic offset instead argv_[4] than
      std::string path_image_replacement =
          helper::File::ResolvePath(path_working_directory_, argv_[4]);

      helper::File::CopyFile(path_image_replacement, path_image_original);

      break;
    }

    if (!found)
      return docxbox::AppLog::NotifyError(
          "Cannot replace " + image_original
              + " - no such image within " + path_docx_in_);

    docxbox::AppLog::NotifyInfo(
        "Replaced " + image_original + " in " + path_docx_in_, true);

    bool overwrite_source_docx;

    // Create resulting DOCX from files during non-batch mode
    // or at final step of batch sequence
    if (is_batch_mode_) {
      if (!is_final_batch_step_) return true;

      overwrite_source_docx = path_docx_in_ == path_docx_out_;
    } else {
      overwrite_source_docx = argc_ < 6;

      path_docx_out_ = overwrite_source_docx
          ? path_docx_in_
          // Result filename is given as argument
          : helper::File::ResolvePath(path_working_directory_, argv_[5]);
    }

    if (!Zip(false, path_extract_, path_docx_out_ + "tmp"))
      return docxbox::AppLog::NotifyError(
          "DOCX creation failed: " + path_docx_out_);

    if (overwrite_source_docx) helper::File::Remove(path_docx_in_.c_str());

    std::rename(
        std::string(path_docx_out_).append("tmp").c_str(),
        path_docx_out_.c_str());
  } catch (std::string &message) {
    return docxbox::AppLog::NotifyError(message);
  }

  return docxbox::AppLog::NotifyInfo("Saved DOCX: " + path_docx_out_);
}

bool docx_archive_replace::ReplaceText() {
  if (!docxbox::AppArgument::AreArgumentsGiven(
      argc_,
      2, "DOCX filename",
      3, "String to be found (and replaced)",
      4, "Replacement")) return false;

  std::string search = argv_[3];
  std::string replacement = argv_[4];

  std::string image_relationship_id;
  std::string hyperlink_relationship_id;

  if (!is_batch_mode_
      && !UnzipDocxByArgv(true, "-" + helper::File::GetTmpName())) return false;

  const char *kReplacement = replacement.c_str();

  try {
    // TODO(kay): single-out render-type detection
    if (helper::String::StartsWith(kReplacement, "{\"image\":{")
        || helper::String::StartsWith(kReplacement, "{\"img\":{")) {
      image_relationship_id = AddImageFileAndRelation(replacement);
    } else if (helper::String::StartsWith(kReplacement, "{\"link\":{")) {
      hyperlink_relationship_id = AddHyperlinkRelation(replacement);
      // TODO(kay): ensure presence of hyperlink-style
    } else if (helper::String::StartsWith(kReplacement, "{\"ol\":{")) {
      std::string path_extract_absolute =
          path_working_directory_ + "/" + path_extract_;
      contentTypes::OverrideNumbering(path_extract_absolute);
    }
  } catch (std::string &message) {
    return docxbox::AppLog::NotifyError(message);
  }

  miniz_cpp::zip_file docx_file(path_docx_in_);

  auto file_list = docx_file.infolist();

  auto parser = new docx_xml_replace(argc_, argv_);
  parser->SetPathExtract(path_extract_);

  if (!image_relationship_id.empty())
    parser->SetImageRelationshipId(image_relationship_id);

  if (!hyperlink_relationship_id.empty())
    parser->SetHyperlinkRelationshipId(hyperlink_relationship_id);

  for (const auto &file_in_zip : file_list) {
    if (!docx_xml::IsXmlFileContainingText(file_in_zip.filename)) continue;

    std::string path_file_absolute = path_extract_ + "/" + file_in_zip.filename;

    if (helper::File::IsDirectory(path_file_absolute)) continue;

    if (!parser->ReplaceInXml(path_file_absolute, search, replacement))
      return docxbox::AppLog::NotifyError(
          "Failed replace string in: " + file_in_zip.filename);

    docxbox::AppLog::NotifyInfo(
        std::to_string(parser->GetAmountReplaced())
        + " replacement(s) done in: " + file_in_zip.filename,
        true);
  }

  delete parser;

  bool overwrite_source_docx;

  // Create resulting DOCX from files during non-batch mode
  // or at final step of batch sequence
  if (is_batch_mode_) {
    if (!is_final_batch_step_) return true;

    overwrite_source_docx = path_docx_in_ == path_docx_out_;
  } else {
    InitPathDocxOutForReplaceText(path_docx_out_, overwrite_source_docx);
  }

  return CreateDocxFromExtract(path_docx_out_, overwrite_source_docx);
}

void docx_archive_replace::InitPathDocxOutForReplaceText(
    std::string &path_docx_out, bool &overwrite_source_docx) const {
  path_docx_out = path_docx_in_;
  overwrite_source_docx = true;

  if (added_image_file_) {
    if (argc_ >= 7) {
      path_docx_out =
          helper::File::ResolvePath(path_working_directory_, argv_[6]);

      overwrite_source_docx = false;
    }
  } else {
    if (argc_ >= 6) {
      path_docx_out =
          helper::File::ResolvePath(path_working_directory_, argv_[5]);


      overwrite_source_docx = false;
    }
  }
}

/**
 * @param image_markup_json
 * @return relationship id, e.g. "rId7"
 */
std::string docx_archive_replace::AddImageFileAndRelation(
  const std::string &image_markup_json) {
  if (!docxbox::AppArgument::isArgImageFile(argc_, argv_, 5))
    // No media file given: successfully done (nothing)
    return "";

  std::string path_extract_absolute =
      helper::File::ResolvePath(path_working_directory_, path_extract_);

  auto relations = new media(path_extract_absolute);

  // 1. Copy image file to word/media/image<number>.<extension>
  if (!relations->AddImageFile(
      helper::File::ResolvePath(path_working_directory_, argv_[5]))) {
    delete relations;

    std::string message = std::string("Failed adding image file ") + argv_[5];

    throw message;
  }

  added_image_file_ = true;

  // 2. Create media relation in _rels/document.xml.rels
  auto relationship_id = relations->GetImageRelationshipId(
      relations->GetMediaPathNewImage());

  delete relations;

  return relationship_id;
}

std::string docx_archive_replace::AddHyperlinkRelation(
    const std::string &markup_json) {
  std::string path_extract_absolute =
      helper::File::ResolvePath(path_working_directory_, path_extract_);

  auto url = helper::Json::GetFirstValueOfKey(markup_json, "url");

  return rels::GetRelationshipId(
      path_extract_absolute,
      url,
      rels::RelationType_Hyperlink);
}

bool docx_archive_replace::RemoveBetweenText() {
  if (!docxbox::AppArgument::AreArgumentsGiven(
      argc_,
      2, "DOCX filename",
      3, "String left-hand-side of part to be removed",
      4, "String right-hand-side of part to be removed")) return false;

  std::string lhs = argv_[3];
  std::string rhs = argv_[4];

  if (!is_batch_mode_
      && !UnzipDocxByArgv(true, "-" + helper::File::GetTmpName())) return false;

  miniz_cpp::zip_file docx_file(path_docx_in_);

  auto file_list = docx_file.infolist();

  auto parser = new docx_xml_remove(argc_, argv_);

  for (const auto &file_in_zip : file_list) {
    if (!docx_xml::IsXmlFileContainingText(file_in_zip.filename)) continue;

    std::string path_file_abs = path_extract_ + "/" + file_in_zip.filename;

    if (!parser->RemoveBetweenStringsInXml(path_file_abs, lhs, rhs)) {
      delete parser;

      return docxbox::AppLog::NotifyError(
          "Error: Failed to remove content from: " + file_in_zip.filename);
    }

    docxbox::AppLog::NotifyInfo(
        std::to_string(parser->GetAmountRemoved())
        + " removal(s) done in: " + file_in_zip.filename,
        true);
  }

  delete parser;

  // Create resulting DOCX from files during non-batch mode
  // or at final step of batch sequence
  if (is_batch_mode_) {
    if (!is_final_batch_step_) return true;
  } else {
    path_docx_out_ =
        argc_ >= 7
        // Result filename is given as argument
        ? helper::File::ResolvePath(
            path_working_directory_,
            argv_[6])
        // Overwrite original DOCX
        : path_docx_in_;
  }

  return CreateDocxFromExtract(path_docx_out_,
                               path_docx_in_ == path_docx_out_);
}

bool docx_archive_replace::ReplaceAllTextByLoremIpsum() {
  if (!is_batch_mode_
      && !UnzipDocxByArgv(true, "-" + helper::File::GetTmpName())) return false;

  miniz_cpp::zip_file docx_file(path_docx_in_);

  auto file_list = docx_file.infolist();

  auto parser = new docx_xml_lorem(argc_, argv_);

  for (const auto &file_in_zip : file_list) {
    if (!docx_xml::IsXmlFileContainingText(file_in_zip.filename)) continue;

    std::string path_file_absolute = path_extract_ + "/" + file_in_zip.filename;

    if (!parser->RandomizeAllTextInXml(path_file_absolute)) {
      delete parser;

      return docxbox::AppLog::NotifyError(
          "Error: Failed insert lorem ipsum in: " + file_in_zip.filename);
    }
  }

  delete parser;

  std::string path_docx_out;
  bool overwrite_source_docx;

  // Create resulting DOCX from files during non-batch mode
  // or at final step of batch sequence
  if (is_batch_mode_) {
    if (!is_final_batch_step_) return true;

    overwrite_source_docx = path_docx_in_ == path_docx_out_;
  } else {
    overwrite_source_docx = argc_ < 4;

    path_docx_out_ = overwrite_source_docx
                    // Overwrite original DOCX
                    ? path_docx_in_
                    // Result filename is given as argument
                    : helper::File::ResolvePath(path_working_directory_,
                                                argv_[3]);
  }

  return CreateDocxFromExtract(path_docx_out_, overwrite_source_docx);
}

bool docx_archive_replace::SetFieldValue() {
  if ((!is_batch_mode_
       && !UnzipDocxByArgv(true, "-" + helper::File::GetTmpName()))
      || !docxbox::AppArgument::AreArgumentsGiven(
      argc_,
      3, "Field identifier",
      4, "Value to be set")) return false;

  std::string field_identifier = argv_[3];
  std::string value = argv_[4];

  miniz_cpp::zip_file docx_file(path_docx_in_);

  auto file_list = docx_file.infolist();
  auto parser = new docx_xml_field(argc_, argv_);

  for (const auto &file_in_zip : file_list) {
    if (!docx_xml::IsXmlFileContainingText(file_in_zip.filename)) continue;

    if (!parser->SetFieldText(
        path_extract_ + "/" + file_in_zip.filename,
        field_identifier,
        value)) {
      delete parser;

      return false;
    }
  }

  delete parser;

  // Create resulting DOCX from files during non-batch mode
  // or at final step of batch sequence
  if (is_batch_mode_) {
    if (!is_final_batch_step_) return true;
  } else {
    path_docx_out_ =
        argc_ >= 6
        // Result filename is given as argument
        ? helper::File::ResolvePath(path_working_directory_, argv_[5])
        // Overwrite original DOCX
        : path_docx_in_;
  }

  std::string path_docx_out_tmp = path_docx_out_ + "tmp";

  if (!Zip(false, path_extract_, path_docx_out_tmp))
    return docxbox::AppLog::NotifyError("DOCX creation failed.");

  return 0 == std::rename(path_docx_out_tmp.c_str(), path_docx_out_.c_str())
         ? docxbox::AppLog::NotifyInfo("Saved DOCX: " + path_docx_out_)
         : docxbox::AppLog::NotifyError(
          "Failed rename temporary DOCX to: " + path_docx_out_);
}
