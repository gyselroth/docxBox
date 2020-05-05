// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_DOCX_DOCX_FILELIST_H_
#define DOCXBOX_DOCX_DOCX_FILELIST_H_

#include <docxbox/docx/docx_archive.h>
#include <docxbox/helper/helper_string.h>

#include <algorithm>
#include <string>
#include <tuple>
#include <vector>

class docx_fileListCompare {
 public:
  static bool CompareLinesByFilenames(std::string str_1, std::string str_2);

  static std::string RenderListsComparison(
      std::string list_1,
      const std::string& summary_1,
      std::string list_2,
      const std::string& summary_2,
      int amount_spaces_gap = 8,
      bool compare_content = false,
      const std::string &path_docx_left = "",
      const std::string &path_docx_right = "");

 private:
  static std::vector<std::string> SplitIntoSortedLines(std::string &file_list);

  static bool IsFileItemLine(const std::string &line);

  static std::string RenderMargin(int len_str, int len_max);

  static bool AreFileInLinesDifferent(
      bool compare_content,
      const std::string &path_extract_left,
      const std::string &path_extract_right,
      const std::string &line_left,
      const std::basic_string<char> &line_right);

  static void UpdateColumnStyles(const std::string &line_left,
                                 const std::string &line_right,
                                 std::string &style_on_left,
                                 std::string &style_on_right,
                                 std::string &style_off);

  static void AdvanceToAlphabeticalNextItem(const std::string &filename_left,
                                            const std::string &filename_right,
                                            int &index_total,
                                            int &index_left,
                                            int &index_right,
                                            std::string &line_left,
                                            std::string &line_right);
};

#endif  // DOCXBOX_DOCX_DOCX_FILELIST_H_
