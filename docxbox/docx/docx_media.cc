// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/docx/docx_media.h>

docx_media::docx_media(std::string path_extract) {
  path_extract_ = std::move(path_extract);
  path_media_ = path_extract_ + "/word/media/";
}

bool docx_media::AddImageFile(const std::string& path_image) {
  std::string number = GetNextImageNumber();

  std::string path_destination =
      path_media_ + "/"
      "image" + number + "." + helper::File::GetExtension(path_image);

  try {
    helper::File::CopyFile(path_image, path_destination);
  } catch (std::string &message) {
    std::cerr << message;

    return false;
  }

  return true;
}

// Get number after highest image-number already given in any image filename
std::string docx_media::GetNextImageNumber() {
  // TODO(kay): implement read-out from image files
  std::string number = "99";

  return number;
}
