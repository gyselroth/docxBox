// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/docx/docx_media.h>

#include <utility>

docx_media::docx_media(std::string path_extract) {
  path_media_ = path_extract + "/word/media";
  path_extract_ = std::move(path_extract);
}

bool docx_media::AddImageFile(const std::string& path_image) {
  std::string number = GetNextImageNumber();

  std::string path_destination =
      path_media_ + "/"
      "image" + number + "." + helper::File::GetExtension(path_image);

  try {
    if (!helper::File::CopyFile(path_image, path_destination)) return false;
  } catch (std::string &message) {
    std::cerr << message;

    return false;
  }

  return true;
}

// Get next highest number, rel. to trailing number within any image filename
std::string docx_media::GetNextImageNumber() {
  auto image_files = helper::File::ScanDir(path_media_.c_str());

  int number = 1;

  for (const auto& filename : image_files) {
    if (!helper::File::IsWordCompatibleImage(filename)) continue;

    std::string no_current = helper::String::ExtractRightMostNumber(filename);

    if (no_current.empty()) continue;

    int number_current = std::stoi(no_current);

    if (number_current > number) number = number_current;
  }

  ++number;

  return std::to_string(number);
}
