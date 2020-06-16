// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_APP_APP_LOG_H_
#define DOCXBOX_APP_APP_LOG_H_

#include <docxbox/helper/helper_dateTime.h>
#include <docxbox/helper/helper_file.h>

#include <iostream>
#include <string>
#include <vector>

namespace docxbox {

static const char *const kFormatDateTimeLog = "%Y-%m-%d %H:%M:%S";

class AppLog {
 public:
  enum LogMode {
    LogTo_File,
    LogTo_FileAndStdOut,
    LogTo_StdOut,
    LogTo_None
  };

  // Create singleton instance of class
  static AppLog* GetInstance();

  // Use GetInstance() instead of constructor
  AppLog();

  static void DeleteInstance();

  // Store given message for later output to stdout / logout
  void Notify(const std::string& message, const std::string& type);

  static bool NotifyError(const std::string &message);
  static bool NotifyInfo(const std::string &message, bool file_only = false);

  // Log docxBox execution arguments to log file
  static void LogStartUp(int argc, char *const *argv);

  static void Output(bool delete_instance = true);
  void OutputToStdOut();
  void OutputToLogFile();

  static bool IsSilent();

 private:
  // Singleton instance pointer
  static AppLog* m_pThis_;

  std::vector<std::string> messages_;
  std::vector<std::string> timestamps_;

  // Messages that are not to be output to stdout, but only to log file
  std::vector<bool> file_only_messages_;

  LogMode mode_ = LogTo_StdOut;

  std::string path_log_file_;

  // Copy constructor
  AppLog(const AppLog&) {}

  // Assignment operator
  AppLog& operator = (const AppLog&) { return *this; }

  void InitMode();
  void InitLogFile();

  // Remember current dateTime (notification occurred)
  void PushBackTime();
};

}  // namespace docxbox

#endif  // DOCXBOX_APP_APP_LOG_H_
