// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/docx/renderer/docx_renderer_image.h>

#include <iostream>

// Constructor
docx_renderer_image::docx_renderer_image(
    int argc,
    char **argv,
    const std::string &json) {
  argc_ = argc;
  argv_ = argv;

  json_ = json;
  is_json_valid_markup_config_ = helper::String::IsJson(json);

  if (is_json_valid_markup_config_) {
    if (argc >= 6) InsertNewImageFile(argv[5]);
    InitSpecsFromJson();
  }
}

bool docx_renderer_image::InsertNewImageFile(const std::string& path_image) {
  // TODO(kay): implement
  return true;
}

std::string docx_renderer_image::RenderMarkup(
    int argc,
    char **argv,
    const std::string& json) {
  auto renderer = new docx_renderer_image(argc, argv, json);

  if (!renderer->Render()) {
    delete renderer;

    throw "Failed render image markup.\n";
  }

  auto markup = renderer->GetWml();

  delete renderer;

  return markup;
}

std::string docx_renderer_image::GetWml() {
  return wml_;
}

// Collect table specs from JSON
void docx_renderer_image::InitSpecsFromJson() {
  if (!helper::String::Contains(json_, "img")) {
    is_json_valid_markup_config_ = false;

    return;
  }

  auto json_outer = nlohmann::json::parse(json_);

  for (auto& json_inner : json_outer) {
    for (nlohmann::json::iterator it = json_inner.begin();
         it != json_inner.end();
         ++it) {
      const std::string& key = it.key();

      if (key == "filename") {
        filename_internal = it.value();
      } else if (key == "offset") {
        auto value = it.value();

        offset_x_ = value.at(0);
        offset_y_ = value.at(1);
      } else if (key == "size") {
        auto value = it.value();

        width_ = value.at(0);
        height_ = value.at(1);
        }
      }
    }

  is_json_valid_markup_config_ =
      width_ > 0
      && height_ > 0;
}


// @see http://officeopenxml.com/drwPic.php
bool docx_renderer_image::Render() {
  if (!is_json_valid_markup_config_) return false;

  wml_ = kWRunLhs;

  wml_ +=
      "<pic:pic "
        "xmlns:pic=\""
          "http://schemas.openxmlformats.org/drawingml/2006/picture\""
      ">"
        "<pic:nvPicPr>"
          "<pic:cNvPr id=\"0\" name=\"" + filename_internal + "\"/>"
          "<pic:cNvPicPr/>"
        "</pic:nvPicPr>"
        "<pic:blipFill>"
          "<a:blip r:embed=\"rId4\" cstate=\"print\"/>"
          "<a:stretch><a:fillRect/></a:stretch/>"
        "</pic:blipFill>"
        "<pic:spPr>"
          "<a:xfrm>"
            "<a:off "
              "x=\"" + std::to_string(offset_x_) + "\" "
              "y=\"" + std::to_string(offset_y_) + "\"/>"
            "<a:ext cx=\"2438400\" cy=\"1828800\"/>"
          "</a:xfrm>"
          "<a:prstGeom rst=\"rect>"
            "<a:avLst/>"
          "</a:prstGeom>"
        "</pic:spPr>"
      "</pic:pic>";

  wml_ += kWRunRhs;

  return true;
}
