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

      // NOTE: Dumped JSON string has associations alphabetically sorted,
      //       and not necessarily in same order as originally given
      arguments_json_.push_back(it.value().dump());
    }
  }

  return !commands_.empty()
         && commands_.size() == arguments_json_.size();
}

bool docx_batch::ProcessSequence() {
  if (!is_json_valid_)
    return docxbox::AppLog::NotifyError(
        "Cannot process: Detected invalid JSON");

  for (int index = 0; index < commands_.size(); ++index)
    if (!ProcessStep(index)) return false;

  return true;
}

bool docx_batch::ProcessStep(int index) {
  // Resolve command + arguments
  std::vector<std::string> app_cli_arguments;

  app_cli_arguments.emplace_back(archive_->GetArgValue(0));
  app_cli_arguments.emplace_back(commands_[index]);
  app_cli_arguments.emplace_back(archive_->GetArgValue(1));

  if (arguments_json_[index] != "[]") {
    auto json_outer = nlohmann::json::parse(arguments_json_[index]);

    for (auto json_inner : json_outer) {
      for (nlohmann::json::iterator it = json_inner.begin();
           it != json_inner.end();
           ++it) {
        nlohmann::json basic_json = it.value();

        if (basic_json.is_object()) {
          app_cli_arguments.emplace_back(std::string(basic_json.dump()));
        } else if (basic_json.is_string()) {
          app_cli_arguments.emplace_back(it.value().dump().c_str());
        } else {
          // TODO(kay): test - needs handling of other types?
        }
      }
    }
  }

  int argc = app_cli_arguments.size();

  // Convert from std::vector<std::string> to char**
  //char **argv = new char*[argc + 1];
  //
  //for (size_t i = 0; i < argc; ++i)
  //  argv[i] = strdup(app_cli_arguments[i].c_str());
  //
  //argv[argc] = nullptr;

  // Init process
  //auto app = new docxbox::App(argc, argv, true);
  auto app = new docxbox::App(argc, app_cli_arguments, true);

  app->SetPathDocxIn(archive_->GetPathDocxIn());
  app->SetPathExtract(archive_->GetPathExtract());

  if (index == commands_.size() - 1) InitFinalBatchStep(app);

  app->Process();

  // delete argv
//  for (size_t i = 0; i < argc; ++i)
//    delete argv[i];  // free memory for each c-style string
//
//  delete[] argv;  // free memory for outer char* array

  delete app;

  return false;
}

// Prepare write resulting DOCX during final step of batch processing
void docx_batch::InitFinalBatchStep(docxbox::App *app) {
  app->SetIsFinalBatchStep(true);

  auto last_argument = archive_->GetLastArgValue();

  app->SetPathDocxOut(
      helper::String::EndsWithCaseInsensitive(last_argument, ".docx")
      // Set given destination path with/or filename for DOCX result file
      ? last_argument
      // Overwrite source DOCX
      : archive_->GetPathDocxIn());
}
