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

  env_var = std::getenv("docxBox_verbose");
  verbose_ = env_var != nullptr && 0 == strcmp(env_var, "1");
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
  clear_log_initially_ = setting_flush_on_start == "1";

  if (clear_log_initially_ || !helper::File::FileExists(path_log_file_))
    helper::File::WriteToNewFile(path_log_file_, "");
}

AppLog* AppLog::GetInstance() {
  if (m_pThis_ == nullptr) m_pThis_ = new AppLog();

  return m_pThis_;
}

void AppLog::DeleteInstance() {
  delete m_pThis_;
}

// Store given message for later output to stdout / logout
void AppLog::Notify(const std::string& message,
                    NotificationType type,
                    bool file_only) {
  if (mode_ == LogTo_None) return;

  file_only_messages_.push_back(verbose_ || file_only);

  if (mode_ == LogTo_File || mode_ == LogTo_FileAndStdOut) PushBackTime();

  std::string type_str = type == NotificationType::Notification_Error
      ? "Error"
      : "Info";

  messages_.push_back("docxBox " + type_str + " - " + message);
}

// Store given error message for later output to stdout / logout
bool AppLog::NotifyError(const std::string& message, bool file_only) {
  GetInstance()->Notify(
      message, NotificationType::Notification_Error, file_only);

  return false;
}

// Store given informational message for later output to stdout / logout
bool AppLog::NotifyInfo(const std::string& message, bool file_only) {
  GetInstance()->Notify(
      message, NotificationType::Notification_Info, file_only);

  return true;
}

// Log docxBox execution arguments to log file
void AppLog::LogStartUp(const std::vector<std::string>& arguments) {
  std::string arg_values;

  for (const auto& argument : arguments) arg_values += argument + " ";

  NotifyInfo("docxBox executing w/ arguments: " + arg_values, true);
}

// Remember current dateTime (notification occurred)
void AppLog::PushBackTime() {
  timestamps_.push_back(
      helper::DateTime::GetCurrentDateTimeFormatted(kFormatDateTimeLog));
}

void AppLog::Output(bool delete_instance) {
  auto kInstance = GetInstance();

  if (kInstance->mode_ != LogTo_None) {
    if (kInstance->mode_ == LogMode::LogTo_StdOut
        || kInstance->mode_ == LogMode::LogTo_FileAndStdOut)
      kInstance->OutputToStdOut();

    if (kInstance->mode_ == LogMode::LogTo_FileAndStdOut
        || kInstance->mode_ == LogMode::LogTo_File)
      kInstance->OutputToLogFile();
  }

  if (delete_instance) DeleteInstance();
}

void AppLog::OutputToStdOut() {
  std::string prev_message;

  int index = -1;

  for (auto &message : messages_) {
    ++index;

    if (message == prev_message
        || file_only_messages_.at(index)) continue;

    if (helper::String::StartsWith(message.c_str(), "docxBox E"))
      std::cerr << message + "\n";
    else
      std::cout << message + "\n";

    prev_message = message;
  }
}

void AppLog::OutputToLogFile() {
  std::string out;
  std::string prev_message;

  int index = -1;

  for (auto &message : messages_) {
    ++index;

    if (message == prev_message) continue;

    out += timestamps_.at(index) + " - " + message.substr(8) + "\n";

    prev_message = message;
  }

  if (!clear_log_initially_) out = "\n" + out;

  helper::File::AppendToFile(path_log_file_, out);
}

bool AppLog::IsSilent() {
  AppLog *kInstance = GetInstance();

  return kInstance->mode_ != LogTo_StdOut
      && kInstance->mode_ != LogTo_FileAndStdOut;
}

}  // namespace docxbox
