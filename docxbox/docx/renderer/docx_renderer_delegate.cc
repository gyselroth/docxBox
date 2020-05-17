// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/docx/renderer/docx_renderer_delegate.h>

std::string docx_renderer_delegate::RenderMarkupFromJson(
    const std::string& json) {
  auto type = docx_renderer::DetectElementType(json);

  std::string markup;

  switch (type) {
    case docx_renderer::Element_Heading1:
      markup = RenderHeading(1, json, markup);
      break;
    case docx_renderer::Element_Heading2:
      markup = RenderHeading(2, json, markup);
      break;
    case docx_renderer::Element_Heading3:
      markup = RenderHeading(3, json, markup);
      break;
    case docx_renderer::Element_Image:
      markup = RenderImage(json, markup);
      break;
    case docx_renderer::Element_ListUnordered:
      // TODO(kay): check presence / insert rel. style
      markup = RenderList(false, json, markup);
      break;
    case docx_renderer::Element_ListOrdered:
      // TODO(kay): check presence / insert rel. style
      markup = RenderList(true, json, markup);
      break;
    case docx_renderer::Element_Table:
      markup = RenderTable(json, markup);
      break;
    case docx_renderer::Element_None:default:
      throw "Invalid markup config, failed to identify element type.";
  }

  return markup;
}
std::string &docx_renderer_delegate::RenderTable(
    const std::string &json, std::string &markup) {
  auto renderer = new docx_renderer_table(argc_, argv_, json);

  replacement_xml_first_child_tag_ = "w:r";

  markup = renderer->Render();

  delete renderer;

  return markup;
}

std::string &docx_renderer_delegate::RenderImage(const std::string &json,
                                           std::string &markup) {
  auto renderer = new docx_renderer_image(argc_, argv_, json);
  renderer->SetRelationshipId(image_relationship_id_);

  replacement_xml_first_child_tag_ = "w:r";

  markup = renderer->Render();

  delete renderer;

  return markup;
}

std::string &docx_renderer_delegate::RenderHeading(
    int level, const std::string &json, std::string &markup) {
  auto renderer = new docx_renderer_heading(argc_, argv_, json);
  renderer->SetLevel(level);

  replacement_xml_first_child_tag_ = "w:p";

  markup = renderer->Render();

  delete renderer;

  return markup;
}

std::string &docx_renderer_delegate::RenderList(
    bool is_ordered, const std::string &json, std::string &markup) {
  auto renderer = new docx_renderer_list(argc_, argv_, json);
  renderer->SetIsOrdered(is_ordered);

  replacement_xml_first_child_tag_ = "w:r";

  markup = renderer->Render();

  delete renderer;

  return markup;
}
