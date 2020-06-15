// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_APP_APP_LOG_H_
#define DOCXBOX_APP_APP_LOG_H_

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

  // Create singleton instance of class
  static AppLog* GetInstance();

  // Use GetInstance() instead of constructor
  AppLog();

  static void DeleteInstance();

  static bool Error(const std::string &message);
  static bool Info(const std::string &message);

  static void Output(bool delete_instance = true);

 private:
  // Singleton instance pointer
  static AppLog* m_pThis_;

  std::vector<std::string> messages_;
  // TODO(kay): collect dateTimes of notifications
  //            and prefix messages in out.log with them
  std::vector<std::string> timestamps_;

  LogMode mode_ = LogTo_StdOut;

  std::string path_log_file_;

 private:
  // Copy constructor
  AppLog(const AppLog&){};

  // Assignment operator
  AppLog& operator=(const AppLog&){ return *this; };

  void InitMode();
  void InitLogFile();
};

}  // namespace docxbox

#endif  // DOCXBOX_APP_APP_LOG_H_
