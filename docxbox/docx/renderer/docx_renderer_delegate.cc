// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/docx/renderer/docx_renderer_delegate.h>

void docx_renderer_delegate::SetPathExtract(const std::string &path_extract) {
  path_extract_ = path_extract;
}

std::string docx_renderer_delegate::RenderMarkupFromJson(
    const std::string& json) {
  auto type = docx_renderer::DetectElementType(json);

  std::string markup;

  switch (type) {
    case docx_renderer::ElementType_Heading1:
      markup = RenderHeading(1, json, markup);
      break;
    case docx_renderer::ElementType_Heading2:
      markup = RenderHeading(2, json, markup);
      break;
    case docx_renderer::ElementType_Heading3:
      markup = RenderHeading(3, json, markup);
      break;
    case docx_renderer::ElementType_Image:
      markup = RenderImage(json, markup);
      break;
    case docx_renderer::ElementType_Link:
      // TODO(kay): add differentiation among links to URL and bookmark
      markup = RenderHyperlink(json, markup);
      break;
    case docx_renderer::ElementType_ListUnordered:
      markup = RenderList(false, json, markup);
      break;
    case docx_renderer::ElementType_ListOrdered:
      markup = RenderList(true, json, markup);
      break;
    case docx_renderer::ElementType_Paragraph:
      markup = RenderParagraph(json, markup);
      break;
    case docx_renderer::ElementType_Table:
      markup = RenderTable(json, markup);
      break;
    case docx_renderer::ElementType_Text:
      markup = RenderText(json, markup);
      break;
    case docx_renderer::ElementType_None:default:
      docxbox::AppLog::NotifyError(
          "Invalid markup config, failed to identify element type.");
  }

  return markup;
}

std::string &docx_renderer_delegate::RenderTable(const std::string &json,
                                                 std::string &markup) {
  auto renderer = new docx_renderer_table(path_extract_, json);

  replacement_xml_root_tag_ = "w:r";

  markup = renderer->Render();

  delete renderer;

  return markup;
}

std::string &docx_renderer_delegate::RenderImage(const std::string &json,
                                                 std::string &markup) {
  auto renderer = new docx_renderer_image(path_extract_, json);

  replacement_xml_root_tag_ = "w:r";

  markup = renderer->Render(image_relationship_id_);

  delete renderer;

  return markup;
}

std::string &docx_renderer_delegate::RenderHeading(int level,
                                                   const std::string &json,
                                                   std::string &markup) {
  auto renderer = new docx_renderer_heading(path_extract_, json);

  replacement_xml_root_tag_ = "w:p";

  markup = renderer->Render(level);

  delete renderer;

  return markup;
}

std::string &docx_renderer_delegate::RenderText(const std::string &json,
                                                std::string &markup) {
  auto renderer = new docx_renderer_text(path_extract_, json);

  markup = renderer->Render();
  replacement_xml_root_tag_ = renderer->generic_root_tag_;

  delete renderer;

  return markup;
}

std::string &docx_renderer_delegate::RenderParagraph(const std::string &json,
                                                     std::string &markup) {
  auto renderer = new docx_renderer_paragraph(path_extract_, json);

  markup = renderer->Render();
  replacement_xml_root_tag_ = renderer->generic_root_tag_;

  delete renderer;

  return markup;
}

std::string &docx_renderer_delegate::RenderList(bool is_ordered,
                                                const std::string &json,
                                                std::string &markup) {
  auto renderer = new docx_renderer_list(path_extract_, json);

  markup = renderer->Render(is_ordered);
  replacement_xml_root_tag_ = renderer->generic_root_tag_;

  delete renderer;

  return markup;
}

std::string &docx_renderer_delegate::RenderHyperlink(const std::string &json,
                                                     std::string &markup) {
  auto renderer = new docx_renderer_link(path_extract_, json);

  markup = renderer->Render(hyperlink_relationship_id_);
  replacement_xml_root_tag_ = renderer->generic_root_tag_;

  delete renderer;

  return markup;
}
