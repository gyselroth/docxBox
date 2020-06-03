// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_DOCX_DOCX_DIFF_H_
#define DOCXBOX_DOCX_DOCX_DIFF_H_

#include <docxbox/helper/helper_cli.h>
#include <docxbox/helper/helper_string.h>
#include <docxbox/helper/helper_file.h>

#include <iostream>
#include <string>

// Shorthand for viewing diff of specific file from two DOCX archives
class docx_diff {
 public:
  static void OutputUnified(const std::string &path_extract_left,
                            const std::string &path_extract_right,
                            const std::string &file);

  static void OutputSideBySide(const std::string &path_extract_left,
                               const std::string &path_extract_right,
                               const std::string &file);
};

#endif  // DOCXBOX_DOCX_DOCX_DIFF_H_
