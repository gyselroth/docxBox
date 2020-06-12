// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/app/app_log.h>

namespace docxbox {

AppLog* AppLog::m_pThis_ = nullptr;

AppLog* AppLog::GetInstance() {
  if (m_pThis_ == nullptr) m_pThis_ = new AppLog();

  return m_pThis_;
}

void AppLog::DeleteInstance() {
  delete m_pThis_;
}

bool AppLog::Error(const std::string& message) {
  auto instance = GetInstance();

  instance->messages_.push_back("docxBox Error - " + message);

  return false;
}

bool AppLog::Info(const std::string& message) {
  auto instance = GetInstance();

  instance->messages_.push_back("docxBox Info - " + message);

  return true;
}

bool AppLog::Warning(const std::string& message) {
  auto instance = GetInstance();

  instance->messages_.push_back("docxBox Warning - " + message);

  return true;
}

void AppLog::Output(LogMode mode, bool delete_instance) {
  auto instance = GetInstance();

  std::string prev_message;

  for (auto &message : instance->messages_) {
    if (message == prev_message) continue;

    if (mode == LogMode::Mode_Output || mode == LogMode::Mode_OutputAndLog)
      std::cout << message << "\n";

    // TODO(kay): if mode involves that append message to log-file

    prev_message = message;
  }

  if (delete_instance) DeleteInstance();
}

}  // namespace docxbox
