// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/app/app.h>  // note: must be here for forward-declaration
#include <docxbox/docx/docx_batch.h>

class docx_archive_replace;

docx_batch::docx_batch(class docx_archive *archive, std::string json) {
  archive_ = archive;
  json_ = std::move(json);

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
      commands_.push_back(it.key());

      // note: dumped JSON string has associations alphabetically sorted,
      //       and not necessarily in same order as originally given
      arguments_json.push_back(it.value().dump());
    }
  }

  return !commands_.empty()
         && commands_.size() == arguments_json.size();
}

bool docx_batch::ProcessSequence() {
  if (!is_json_valid_)
    return docxbox::AppStatus::Error("Cannot process: Detected invalid JSON");

  for (int index = 0; index < commands_.size(); ++index)
    if (!ProcessStep(index)) return false;

  return true;
}

bool docx_batch::ProcessStep(int index) {
  // TODO(kay): resolve arguments_json_ at index
  //            retrieve 1. current arguments, 2. required argc

  int argc = 3;

  char **argv = new char*[3];

  argv[0] = archive_->GetArgValue(0);
  argv[1] = (char*)commands_[index].c_str();
  //argv[2] = "";

  auto app = new docxbox::App(argc, argv, true);

  // TODO(kay): invoke App::Process()
  // TODO(kay): adapt all DOCX modification methods for optional batch mode

  return false;
}
