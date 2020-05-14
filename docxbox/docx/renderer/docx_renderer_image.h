// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_DOCX_RENDERER_DOCX_RENDERER_IMAGE_H_
#define DOCXBOX_DOCX_RENDERER_DOCX_RENDERER_IMAGE_H_

#include <docxbox/docx/renderer/docx_renderer.h>
#include <docxbox/helper/helper_string.h>

#include <string>
#include <vector>

#include <vendor/json/single_include/nlohmann/json.hpp>

class docx_renderer_image: docx_renderer {
 public:
  // Constructor
  explicit docx_renderer_image(int argc, char **argv, const std::string &json);

  void SetRelationshipId(const std::string &relationship_id);

  std::string Render();

 private:
  // Image specs from JSON
  std::string filename_internal_;

  int offset_x_ = 0;
  int offset_y_ = 0;

  int width_ = 0;
  int height_ = 0;

  std::string relationship_id_;

  static bool InsertNewImageFile(const std::string& path_image);

  void InitSpecsFromJson();
};

#endif  // DOCXBOX_DOCX_RENDERER_DOCX_RENDERER_IMAGE_H_
