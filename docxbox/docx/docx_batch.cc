// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/docx/docx_batch.h>

docx_batch::docx_batch(std::string path_extract, std::string json)
    : path_extract_(std::move(path_extract)), json_(std::move(json)) {
  is_json_valid_ = helper::Json::IsJson(json_);

  if (is_json_valid_) InitFromJson();
}

bool docx_batch::InitFromJson() {
  std::cout << "TODO(kay): implement parse JSON\n";

  return true;
}

bool docx_batch::Process() {
  if (!is_json_valid_)
    return docxbox::AppError::Output("Cannot process: Detected invalid JSON");

  std::cout << "TODO(kay): implement process JSON\n";

  return true;
}
