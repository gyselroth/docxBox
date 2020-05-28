// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_DOCX_RENDERER_CONTENTTYPE_DOCX_RENDERER_IMAGE_H_
#define DOCXBOX_DOCX_RENDERER_CONTENTTYPE_DOCX_RENDERER_IMAGE_H_

#include <docxbox/docx/renderer/docx_renderer.h>
#include <docxbox/helper/helper_string.h>

#include <string>
#include <utility>
#include <vector>

#include <vendor/json/single_include/nlohmann/json.hpp>

class docx_renderer_image: public docx_renderer {
 public:
  // Constructor
  explicit docx_renderer_image(
      std::string path_extract, const std::string &json);

  void SetRelationshipId(const std::string &relationship_id);

  std::string Render(const std::string& image_relationship_id);
  std::string Render() override;

 private:
  // Image specs from JSON
  std::string image_name;

  int offset_x_ = 0;
  int offset_y_ = 0;

  int width_ = 0;
  int height_ = 0;

  std::string relationship_id_;

  bool InitFromJson() override;
};

#endif  // DOCXBOX_DOCX_RENDERER_CONTENTTYPE_DOCX_RENDERER_IMAGE_H_
