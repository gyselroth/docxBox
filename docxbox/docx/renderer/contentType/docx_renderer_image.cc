// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/docx/renderer/contentType/docx_renderer_image.h>

// Constructor
docx_renderer_image::docx_renderer_image(std::string path_extract,
                                         const std::string &json) {
  path_extract_ = std::move(path_extract);

  json_ = json;
  is_json_valid_ = InitFromJson();
}

void docx_renderer_image::SetRelationshipId(
    const std::string &relationship_id) {
  relationship_id_ = relationship_id;
}

bool docx_renderer_image::InitFromJson() {
  if (!helper::Json::IsJson(json_)
      || !docx_renderer::IsElementType(ElementType_Image)) return false;

  auto json_outer = nlohmann::json::parse(json_);

  for (auto& json_inner : json_outer) {
    for (nlohmann::json::iterator it = json_inner.begin();
         it != json_inner.end();
         ++it) {
      const std::string& key = it.key();

      if (key == "name") {
        image_name = it.value();
      } else if (key == "offset") {
        auto value = it.value();

        offset_x_ = value.at(0);
        offset_y_ = value.at(1);
      } else if (key == "size") {
        auto value = it.value();

        try {
          width_ = value.at(0);
        } catch (nlohmann::detail::type_error &error) {
          // Fallback: Value not given as int, assume it being a string
          std::string width_str = value.at(0);

          width_ = (helper::String::EndsWith(width_str, "px"))
                   ? PixelsToEmus(width_str)
                   : helper::Numeric::ExtractLeadingNumber(width_str);
        }

        try {
          height_ = value.at(1);
        } catch (nlohmann::detail::type_error &error) {
          // Fallback: Value not given as int, assume it being a string
          std::string height_str = value.at(1);

          height_ = (helper::String::EndsWith(height_str, "px"))
                   ? PixelsToEmus(height_str)
                   : helper::Numeric::ExtractLeadingNumber(height_str);
        }
      } else if (key == "pre" || key == "post") {
        ExtractPreOrPostfix(it);
      }
    }
  }

  return width_ > 0 && height_ > 0;
}

std::string docx_renderer_image::Render(
    const std::string& image_relationship_id) {
  SetRelationshipId(image_relationship_id);

  return Render();
}

// @see http://officeopenxml.com/drwPic.php
std::string docx_renderer_image::Render() {
  if (!is_json_valid_) {
    docxbox::AppLog::NotifyError("Failed render image markup");

    return "";
  }

  generic_root_tag_ = "w:r";

  wml_ = TAG_LHS_RUN;

  // TODO(kay): insert dimension from JSON -> a:ext...

  wml_ +=
    "<w:rPr><w:noProof/></w:rPr>"
    "<w:drawing>"
      "<wp:inline distT=\"0\" distB=\"0\" distL=\"0\" distR=\"0\">"
        "<wp:extent cx=\"3810000\" cy=\"3810000\"/>"
        "<wp:effectExtent l=\"0\" t=\"0\" r=\"0\" b=\"0\"/>"
        "<wp:docPr id=\"1\" name=\"Picture1\"/>"
        "<wp:cNvGraphicFramePr>\n"
          "<a:graphicFrameLocks "
            "xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" "
            "noChangeAspect=\"1\"/>"
        "</wp:cNvGraphicFramePr>"
        "<a:graphic "
          "xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\""
        ">"
          "<a:graphicData "
            "uri=\"http://schemas.openxmlformats.org/drawingml/2006/picture\""
          ">"
            "<pic:pic "
            "xmlns:pic=\""
              "http://schemas.openxmlformats.org/drawingml/2006/picture\""
            ">"

              "<pic:nvPicPr>"
                "<pic:cNvPr id=\"0\" name=\"" + image_name + "\"/>"
                "<pic:cNvPicPr/>"
              "</pic:nvPicPr>"
            "<pic:blipFill>"
              "<a:blip r:embed=\"" + relationship_id_ + "\" cstate=\"print\"/>"
              "<a:stretch>"
                "<a:fillRect/>"
              "</a:stretch>"
            "</pic:blipFill>"
            "<pic:spPr>"
              "<a:xfrm>"
                "<a:off "
                  "x=\"" + std::to_string(offset_x_) + "\" "
                  "y=\"" + std::to_string(offset_y_) + "\"/>"
                "<a:ext cx=\"2438400\" cy=\"1828800\"/>"
              "</a:xfrm>"
              "<a:prstGeom rst=\"rect\">"
                "<a:avLst/>"
              "</a:prstGeom>"
  //            "<a:ln w=\"12700\"><a:noFill/></a:ln>"
            "</pic:spPr>"
          "</pic:pic>"

          "</a:graphicData>"
        "</a:graphic>"
      "</wp:inline>"
    "</w:drawing>";

  wml_ += TAG_RHS_RUN;

  RenderPreAndPostFixAroundWml();

  return wml_;
}
