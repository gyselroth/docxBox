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

class AppLog {
 public:
  enum LogMode {
    LogTo_File,
    LogTo_FileAndStdOut,
    LogTo_StdOut,
    LogTo_None
  };

  enum NotificationType {
    Notification_Error,
    Notification_Warning,
    Notification_Info
  };

  // Create singleton instance of class
  static AppLog* GetInstance();

  // Use GetInstance() instead of constructor
  AppLog();

  static void DeleteInstance();

  // Store given message for later output to stdout / logout
  void Notify(const std::string& message,
              NotificationType type,
              bool file_only = false);

  static bool NotifyError(const std::string &message, bool file_only = false);
  static bool NotifyWarning(const std::string &message, bool file_only = false);
  static bool NotifyInfo(const std::string &message, bool file_only = false);

  // Log docxBox execution arguments to log file
  static void LogStartUp(const std::vector<std::string>& arguments);

  static void Output(bool delete_instance = true);
  void OutputToStdOut();
  void OutputToLogFile();

  static bool IsSilent();

 private:
  static const char FORMAT_TIMESTAMP[];

  // Singleton instance pointer
  static AppLog* m_pThis_;

  std::vector<std::string> messages_;
  std::vector<std::string> timestamps_;

  // Messages that are not to be output to stdout, but only to log file
  std::vector<bool> file_only_messages_;

  LogMode mode_ = LogTo_StdOut;

  std::string path_log_file_;

  bool clear_log_initially_ = false;
  bool verbose_ = false;

  // Copy constructor
  AppLog(const AppLog&) {}

  // Assignment operator
  AppLog& operator = (const AppLog&) { return *this; }

  void InitMode();
  void InitLogFile();

  static bool MessageTypeExtortsOutput(const std::string &message);

  // Remember current dateTime (notification occurred)
  void PushBackTime();
};

}  // namespace docxbox

#endif  // DOCXBOX_APP_APP_LOG_H_
