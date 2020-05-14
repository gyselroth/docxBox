// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_DOCX_DOCX_MEDIA_H_
#define DOCXBOX_DOCX_DOCX_MEDIA_H_

#include <docxbox/docx/xml/docx_xml_rels.h>
#include <docxbox/helper/helper_file.h>

#include <string>
#include <utility>

class docx_media {
 public:
  explicit docx_media(std::string path_extract);

  bool AddImageFile(const std::string& path_image);

  // Get (add if not exists) image relationship in _rels/document.xml.rels
  std::string GetImageRelationshipId(const std::string &path_image);

  std::string GetMediaPathNewImage();

 private:
  std::string path_extract_;

 private:
  std::string path_media_;

  std::string media_path_new_image_ = "";

  std::string GetNextImageNumber();
};

#endif  // DOCXBOX_DOCX_DOCX_MEDIA_H_
