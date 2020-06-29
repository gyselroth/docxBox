// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/docx/component/media.h>

#include <utility>

media::media(std::string path_extract) {
  path_media_ = path_extract + "/word/media";
  path_extract_ = std::move(path_extract);
}

bool media::AddImageFile(const std::string& path_image) {
  std::string number = GetNextImageNumber();

  std::string filename_image =
      "image" + number + "." + helper::File::GetExtension(path_image);

  std::string path_destination = path_media_ + "/" + filename_image;

  if (!helper::File::IsDir(path_media_)) mkdir(path_media_.c_str(), 0777);

  try {
    if (!helper::File::CopyFile(path_image, path_destination)) return false;
  } catch (std::string &message) {
    return docxbox::AppLog::NotifyError(message);
  }

  media_path_new_image_ = "media/" + filename_image;

  return true;
}

// Get next highest number, rel. to trailing number within any image filename
std::string media::GetNextImageNumber() {
  auto image_files = helper::File::ScanDir(path_media_.c_str());

  int number = 1;

  for (const auto& filename : image_files) {
    if (!helper::File::IsWordCompatibleImage(filename)) continue;

    std::string no_current = helper::String::ExtractTrailingNumber(filename);

    if (no_current.empty()) continue;

    int number_current = std::stoi(no_current);

    if (number_current > number) number = number_current;
  }

  ++number;

  return std::to_string(number);
}

// Get (add if not exists) relationship into _rels/document.xml.rels
std::string media::GetImageRelationshipId(const std::string &target) {
  auto rels = new docx_xml_rels(path_extract_);
  auto relationship_id = rels->GetRelationShipIdByTarget(
      target, rels::RelationType_Image);

  delete rels;

  return relationship_id;
}

std::string media::GetMediaPathNewImage() {
  return media_path_new_image_;
}
