// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_APP_APP_STATUS_H_
#define DOCXBOX_APP_APP_STATUS_H_

#include <iostream>
#include <string>
#include <vector>

namespace docxbox {

class AppLog {
 public:
  enum LogMode {
    Mode_Log,
    Mode_Output,
    Mode_OutputAndLog
  };

  // Create singleton instance of class
  static AppLog* GetInstance();
  static void DeleteInstance();

  static bool Error(const std::string &message);
  static bool Info(const std::string &message);
  static bool Warning(const std::string &message);

  static void Output(LogMode mode = LogMode::Mode_Output, bool delete_instance = true);

 private:
  // Singleton instance pointer
  static AppLog* m_pThis_;

  std::vector<std::string> messages_;

  // Default constructor
  AppLog() = default;;

  // Copy constructor
  AppLog(const AppLog&){};

  // Assignment operator
  AppLog& operator=(const AppLog&){ return *this; };
};

}  // namespace docxbox

#endif  // DOCXBOX_APP_APP_STATUS_H_
