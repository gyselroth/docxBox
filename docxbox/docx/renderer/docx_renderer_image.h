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

  std::string GetWml();

  static std::string RenderMarkup(
      int argc,
      char **argv,
      const std::string& json);

  bool Render();

 private:
  // Image specs from JSON
  std::string filename_internal;

  int offset_x_ = 0;
  int offset_y_ = 0;

  int width_ = 0;
  int height_ = 0;

  static bool InsertNewImageFile(const std::string& path_image);

  void InitSpecsFromJson();
};

#endif  // DOCXBOX_DOCX_RENDERER_DOCX_RENDERER_IMAGE_H_
