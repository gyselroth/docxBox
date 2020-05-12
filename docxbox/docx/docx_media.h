// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_DOCX_DOCX_CONTENTTYPES_H_
#define DOCXBOX_DOCX_DOCX_CONTENTTYPES_H_

#include <string>
#include <utility>

#include <docxbox/helper/helper_file.h>

class docx_media {
 public:
  explicit docx_media(std::string path_extract);

  bool AddImageFile(const std::string& path_image);

 private:
  std::string path_extract_;
  std::string path_media_;

  std::string GetNextImageNumber();
};

#endif  // DOCXBOX_DOCX_DOCX_CONTENTTYPES_H_
