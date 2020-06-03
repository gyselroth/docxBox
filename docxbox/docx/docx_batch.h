// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_DOCX_DOCX_BATCH_H_
#define DOCXBOX_DOCX_DOCX_BATCH_H_

#include <docxbox/app/app_error.h>
#include <docxbox/helper/helper_cli.h>
#include <docxbox/helper/helper_string.h>
#include <docxbox/helper/helper_file.h>
#include <docxbox/helper/helper_json.h>

#include <iostream>
#include <string>
#include <utility>

class docx_batch {
 public:
  docx_batch(std::string path_extract, std::string json);

  bool Process();

 private:
  bool is_json_valid_;

  std::string path_extract_;
  std::string json_;

  bool InitFromJson();
};

#endif  // DOCXBOX_DOCX_DOCX_BATCH_H_
