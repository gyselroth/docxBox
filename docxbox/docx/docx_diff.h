// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_DOCX_ARCHIVE_DOCX_DIFF_H_
#define DOCXBOX_DOCX_ARCHIVE_DOCX_DIFF_H_

#include <docxbox/helper/helper_cli.h>
#include <docxbox/helper/helper_string.h>
#include <docxbox/helper/helper_file.h>

#include <iostream>
#include <string>

class docx_diff {
 public:
  static void OutputUnified(const std::string &path_extract_left,
                            const std::string &path_extract_right,
                            const std::string &file);

  static void OutputSideBySide(const std::string &path_extract_left,
                               const std::string &path_extract_right,
                               const std::string &file);
};

#endif  // DOCXBOX_DOCX_ARCHIVE_DOCX_DIFF_H_
