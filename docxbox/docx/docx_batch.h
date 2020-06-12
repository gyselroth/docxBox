// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_DOCX_DOCX_BATCH_H_
#define DOCXBOX_DOCX_DOCX_BATCH_H_

#include <docxbox/app/app_status.h>
#include <docxbox/app/app_command.h>
#include <docxbox/helper/helper_cli.h>
#include <docxbox/helper/helper_string.h>
#include <docxbox/helper/helper_file.h>
#include <docxbox/helper/helper_json.h>

#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace docxbox {
class App;  // forward-declare docxbox::App
}

class docx_batch {
 public:
  docx_batch(class docx_archive *archive, std::string json);

  bool ProcessSequence();

 private:
  bool is_json_valid_;

  docx_archive *archive_;

  std::string json_;

  // Batch commands. E.g.: {"rpt","rpi",...}
  std::vector<std::string> commands_;

  // Arguments of batch commands, as JSON strings.
  // E.g: {
  //       "[\"foo\",{"h1":"{"text": "Heading"}}"]",
  //       "[...
  //      }
  std::vector<std::string> arguments_json_;

  bool InitFromJson();

  bool ProcessStep(int index);

  // Prepare write resulting DOCX during final step of batch processing
  void InitFinalBatchStep(docxbox::App *app);
};

#endif  // DOCXBOX_DOCX_DOCX_BATCH_H_
