// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/app/app.h>  // forward-declaration

#include <docxbox/docx/docx_batch.h>

class docx_archive_replace;

docx_batch::docx_batch(class docx_archive *archive, std::string json) {
  archive_ = archive;
  json_ = std::move(json);

  is_json_valid_ = InitFromJson();
}

// Extract batch commands and rel. arguments
bool docx_batch::InitFromJson() {
  try {
    auto json_outer = nlohmann::json::parse(json_);

    for (auto json_inner : json_outer) {  // Sequence keys e.g "1", "2", ...
      for (nlohmann::json::iterator it = json_inner.begin();  // Commands + JSON
           it != json_inner.end();
           ++it) {
        commands_.push_back(it.key());

        // NOTE: Dumped JSON string has associations alphabetically sorted,
        //       not necessarily same order as originally given
        const std::basic_string<char> &kArgsJson = it.value().dump();

        arguments_json_.push_back(kArgsJson);
      }
    }

    return !commands_.empty()
        && commands_.size() == arguments_json_.size();
  } catch (nlohmann::detail::parse_error &e) {
    return docxbox::AppLog::NotifyError("Parse error - Invalid JSON: " + json_);
  }
}

bool docx_batch::ProcessSequence() {
  if (!is_json_valid_)
    return docxbox::AppLog::NotifyError(
        "Cannot process: Detected invalid JSON");

  int index = 0;

  for (auto command : commands_) {
    if (!ProcessStep(index)) return false;
    ++index;
  }

  return true;
}

bool docx_batch::ProcessStep(int index) {
  if (index == 0) AddImagesIntoDocument(archive_->GetPathExtract());

  docxbox::AppLog::NotifyInfo("Batch-process step " + std::to_string(index));

  // Extract command of process step
  std::vector<std::string> app_cli_arguments;

  app_cli_arguments.emplace_back(archive_->GetArgValueAt(0));
  app_cli_arguments.emplace_back(commands_[index]);
  app_cli_arguments.emplace_back(archive_->GetArgValueAt(1));

  if (arguments_json_[index] != "[]") {
    // Extract arguments of process step into app_cli_arguments<>
    auto json_outer = nlohmann::json::parse(arguments_json_[index]);

    for (auto json_inner : json_outer) {
      for (nlohmann::json::iterator it = json_inner.begin();
           it != json_inner.end();
           ++it) {
        nlohmann::json basic_json = it.value();

        if (basic_json.is_object()) {
          std::string object_json =
              std::string("{\"")
              .append(it.key()).append("\":")
              .append(std::string(basic_json.dump()))
              .append("}");

          app_cli_arguments.emplace_back(object_json);
        } else if (basic_json.is_string()) {
          app_cli_arguments.emplace_back(it.value().dump().c_str());
        }
        // TODO(kay): test - needs handling of other types?
      }
    }
  }

  int argc = app_cli_arguments.size();

  // Init process
//  auto app = new docxbox::App(argc, argv, true);
  auto app = new docxbox::App(argc, app_cli_arguments, true);

  app->SetPathDocxIn(archive_->GetPathDocxIn());
  app->SetPathExtract(archive_->GetPathExtract());

  if (index == commands_.size() - 1) InitFinalStep(app);

  app->Process();

  delete app;

  return true;
}

bool docx_batch::AddImagesIntoDocument(const std::string &path_extract) {
  auto arguments = archive_->GetArgv();
  rels *kRels = new rels(0, {}, getenv("PWD"), path_extract);

  bool added_image = false;

  for (auto &argument : arguments) {
    if (helper::File::IsWordCompatibleImage(argument)
        && !kRels->AddImageFileAndRelation(&argument).empty()) {
      docxbox::AppLog::NotifyInfo("Added image file and relation: " + argument);
      added_image = true;
    }
  }

  delete kRels;

  return added_image;
}

// Prepare write resulting DOCX during final step of batch processing
void docx_batch::InitFinalStep(docxbox::App *app) {
  app->SetIsFinalBatchStep(true);

  auto last_argument = archive_->GetLastArgValue();

  app->SetPathDocxOut(
      helper::String::EndsWithCaseInsensitive(last_argument, ".docx")
      // Set given destination path with/or filename for DOCX result file
      ? last_argument
      // Overwrite source DOCX
      : archive_->GetPathDocxIn());
}

std::string docx_batch::ExtractImageFilenameFromBatchStepJson(
    const std::vector<std::string> &arguments) {
  for (auto argument : arguments) {
    helper::String::ReplaceAll(&argument, " ", "");

    if (!helper::Json::IsJson(argument)
        || !helper::String::Contains(argument, "\"name\":")) continue;

    return helper::String::GetSubStrBetween(argument, R"("name":")", "\",");
  }

  return "";
}
