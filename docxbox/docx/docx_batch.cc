// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/docx/docx_batch.h>

docx_batch::docx_batch(std::string path_extract, std::string json)
    : path_extract_(std::move(path_extract)), json_(std::move(json)) {
  is_json_valid_ = InitFromJson();
}

// Extract batch commands and rel. arguments
bool docx_batch::InitFromJson() {
  if (!helper::Json::IsJson(json_)) return false;

  auto json_outer = nlohmann::json::parse(json_);

  for (auto json_inner : json_outer) {
    for (nlohmann::json::iterator it = json_inner.begin();
         it != json_inner.end();
         ++it) {
      commands_.push_back(docxbox::AppCommands::ResolveCommandByName(it.key()));

      // note: dumped JSON string has associations alphabetically sorted,
      //       and not necessarily in same order as originally given
      arguments_json.push_back(it.value().dump());
    }
  }

  return !commands_.empty()
         && commands_.size() == arguments_json.size();
}

bool docx_batch::Process() {
  if (!is_json_valid_)
    return docxbox::AppError::Output("Cannot process: Detected invalid JSON");

  std::cout << "TODO(kay): implement process JSON\n";

  return true;
}
