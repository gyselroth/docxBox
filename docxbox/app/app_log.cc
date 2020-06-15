// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/app/app_log.h>

namespace docxbox {

AppLog* AppLog::m_pThis_ = nullptr;

AppLog::AppLog() {
  InitMode();

  if (mode_ != LogTo_StdOut && mode_ != LogTo_None) InitLogFile();
}

void AppLog::InitMode() {
  auto env_var = std::getenv("docxBox_notify");

  std::string option = env_var == nullptr
      ? ""
      : std::string(env_var);

  if (option.empty() || option == "stdout") {
    return;
  }

  if (option == "log") {
    mode_ = LogMode::LogTo_File;
  } else if (option == "both") {
    mode_ = LogMode::LogTo_FileAndStdOut;
  } else if (option == "off") {
    mode_ = LogMode::LogTo_None;
  }
}

void AppLog::InitLogFile() {
  auto env_var = std::getenv("docxBox_log_path");

  std::string setting_path = env_var == nullptr
      ? ""
      : std::string(env_var);

  path_log_file_ = setting_path.empty()
      ? std::string(getenv("PWD")) + "/out.log"
      : setting_path;

  env_var = std::getenv("docxBox_clear_log_on_start");

  if (env_var == nullptr) return;

  std::string setting_flush_on_start = std::string(env_var);

  if (setting_flush_on_start == "1"
      || !helper::File::FileExists(path_log_file_))
    helper::File::WriteToNewFile(path_log_file_, "");
}

AppLog* AppLog::GetInstance() {
  if (m_pThis_ == nullptr) m_pThis_ = new AppLog();

  return m_pThis_;
}

void AppLog::DeleteInstance() {
  delete m_pThis_;
}

bool AppLog::Error(const std::string& message) {
  AppLog *kInstance = GetInstance();

  if (kInstance->mode_ != LogTo_None)
    kInstance->messages_.push_back("docxBox Error - " + message);

  return false;
}

bool AppLog::Info(const std::string& message) {
  AppLog *kInstance = GetInstance();

  if (kInstance->mode_ != LogTo_None)
    kInstance->messages_.push_back("docxBox Info - " + message);

  return true;
}

void AppLog::Output(bool delete_instance) {
  auto kInstance = GetInstance();

  if (kInstance->mode_ != LogTo_None) {
    std::string prev_message;
    std::string out;

    for (auto &message : kInstance->messages_) {
      if (message == prev_message) continue;

      out += message + "\n";

      prev_message = message;
    }

    if (kInstance->mode_ == LogMode::LogTo_StdOut
        || kInstance->mode_ == LogMode::LogTo_FileAndStdOut) std::cout << out;

    if (kInstance->mode_ == LogMode::LogTo_FileAndStdOut
        || kInstance->mode_ == LogMode::LogTo_File) {
      helper::File::AppendToFile(kInstance->path_log_file_, out);
    }
  }

  if (delete_instance) DeleteInstance();
}

}  // namespace docxbox
