// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_APP_APP_H_
#define DOCXBOX_APP_APP_H_

#include <docxbox/app/app_argument.h>
#include <docxbox/app/app_command.h>
#include <docxbox/app/app_help.h>
#include <docxbox/config.h>
#include <docxbox/docx/archive/docx_archive.h>
#include <docxbox/docx/archive/docx_archive_list.h>
#include <docxbox/docx/archive/docx_archive_replace.h>
#include <docxbox/helper/helper_dateTime.h>

#include <cstring>
#include <iostream>
#include <string>
#include <vector>

namespace docxbox {

class App {
 public:
  // Meta constants
  static const std::string kAppName;
  static const std::string kAppExecutableName;

  // Constructor: init (resolve) command and arguments
  App(int argc,
      const std::vector<std::string>& argv,
      bool is_batch_mode = false);

  virtual ~App();

  void SetPathDocxIn(const std::string &path_docx_in);
  void SetPathDocxOut(const std::string &path_docx_out);
  void SetPathExtract(const std::string &path_extract);

  void SetIsFinalBatchStep(bool is_final_batch_step);

  // Process command + arguments
  bool Process();

 private:
  int argc_;
  std::vector<std::string> argv_;

  bool is_batch_mode_ = false;

  AppCommands *command_;

  // Override-paths to be passed-on to archive-object during batch processing
  std::string path_docx_in_;
  std::string path_extract_;

  // Only upon final step of batch sequence, modification methods need to
  // zip modified files back into the resulting DOCX
  bool is_final_batch_step_ = false;

  // DOCX output path with/or filename
  std::string path_docx_out_;

  AppCommands::Command PreProcess(AppArgument *arguments,
                                  const AppCommands::Command &command) const;

  bool ProcessList(AppCommands::Command command);
  bool ProcessReplace(AppCommands::Command command);

  void InitBatchProcessor(docx_archive *docx_archive) const;
};

}  // namespace docxbox

#endif  // DOCXBOX_APP_APP_H_
