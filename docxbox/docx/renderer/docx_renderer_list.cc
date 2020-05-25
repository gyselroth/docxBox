// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/docx/renderer/docx_renderer_list.h>
#include <docxbox/docx/component/contentTypes.h>

// Constructor
docx_renderer_list::docx_renderer_list(
    std::string path_extract, const std::string &json) {
  path_extract_ = std::move(path_extract);

  json_ = json;
  is_json_valid_ = InitFromJson();
}

void docx_renderer_list::SetIsOrdered(bool is_ordered) {
  is_ordered_ = is_ordered;
}

void docx_renderer_list::AddNumberingXml() const {
  if (is_json_valid_) {
    auto numbering_component = new numbering(path_extract_);
    numbering_component->AddNumberingXml(is_ordered_);

    delete numbering_component;
  }
}

bool docx_renderer_list::InitFromJson() {
  if (!helper::String::IsJson(json_)
      || !docx_renderer::IsElementType(
          {ElementType_ListUnordered, ElementType_ListOrdered})) return false;

  items_.clear();

  auto json_outer = nlohmann::json::parse(json_);

  for (auto &json_inner : json_outer) {
    for (nlohmann::json::iterator it = json_inner.begin();
         it != json_inner.end();
         ++it) {
      try {
        const std::string &key = it.key();

        if ("items" == key) {
          auto items = it.value();

          for (nlohmann::json::iterator it_items = items.begin();
               it_items != items.end();
               ++it_items)
            items_.push_back(it_items.value());
        }
      } catch (nlohmann::detail::invalid_iterator &e) {
        continue;
      }
    }
  }

  return items_.empty()
    ? docxbox::AppError::Output("Invalid markup: list contains no items.")
    : true;
}

std::string docx_renderer_list::Render(bool is_ordered) {
  SetIsOrdered(is_ordered);
  AddNumberingXml();
  // TODO(kay): implement add specs
  //  for ordered-list in word/_rels/document.xml.rels

  return Render();
}

std::string docx_renderer_list::Render() {
  if (!is_json_valid_) throw "Failed render list markup.\n";

  wml_ = kWRunLhs;

  for (const std::string& item : items_) {
    wml_ +=
        "<w:p>"
          "<w:pPr>"
            "<w:numPr>"
              "<w:ilvl w:val=\"0\"/>"
              "<w:numId w:val=\"1\"/>"
            "</w:numPr>"
            "<w:ind w:left=\"360\" w:hanging=\"360\"/>"
          "</w:pPr>"
          "<w:r>"
            "<w:t>" + item + "</w:t>"
          "</w:r>"
        "</w:p>";
  }

  wml_ += kWRunRhs;

  return wml_;
}
