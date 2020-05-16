// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/docx/renderer/docx_renderer_list.h>

#include <iostream>

// Constructor
docx_renderer_list::docx_renderer_list(
    int argc, char **argv, const std::string &json) {
  argc_ = argc;
  argv_ = argv;

  json_ = json;
  is_json_valid_markup_config_ = helper::String::IsJson(json);

  if (is_json_valid_markup_config_) {
    InitSpecsFromJson();
  }
}

void docx_renderer_list::SetIsOrdered(bool is_ordered) {
  is_ordered_ = is_ordered;
}

void docx_renderer_list::InitSpecsFromJson() {
  if (!docx_renderer::IsValidJsonForList(json_)) {
    is_json_valid_markup_config_ = false;

    return;
  }

  auto json_outer = nlohmann::json::parse(json_);

  for (auto &json_inner : json_outer) {
    for (nlohmann::json::iterator it = json_inner.begin();
         it != json_inner.end();
         ++it) {
      const std::string &key = it.key();

      if (key == "items") {
        // TODO(kay): fetch items
        //text_ = it.value();
      }
    }
  }
}

std::string docx_renderer_list::Render() {
  if (!is_json_valid_markup_config_) throw "Failed render list markup.\n";

  //wml_ = kWRunLhs;

  // TODO(kay): add markup
  wml_ = "";

  //wml_ += kWRunRhs;

  return wml_;
}
