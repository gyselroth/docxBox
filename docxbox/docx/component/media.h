// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_DOCX_COMPONENT_MEDIA_H_
#define DOCXBOX_DOCX_COMPONENT_MEDIA_H_

#include <docxbox/app/app_log.h>
#include <docxbox/docx/xml/component/docx_xml_rels.h>
#include <docxbox/helper/helper_file.h>

#include <string>
#include <utility>

class media {
 public:
  explicit media(std::string path_extract);

  // Copy given image file into media/<file>, set media_path_new_image_
  bool AddImageFile(const std::string& path_image);

  // Get (add if not exists) relationship in _rels/document.xml.rels
  std::string GetRelationshipId(const std::string &target);

  std::string GetMediaPathNewImage();

 private:
  std::string path_extract_;

 private:
  std::string path_media_;

  std::string media_path_new_image_ = "";

  std::string GetNextImageNumber();
};

#endif  // DOCXBOX_DOCX_COMPONENT_MEDIA_H_
