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
  docx_fileListCompare(
      std::string list_1, std::string summary_1,
      std::string list_2, std::string summary_2,
      std::string path_docx_in_1, std::string path_docx_in_2);

  static bool CompareLinesByFilenames(std::string str_1, std::string str_2);

  void Output();

 private:
  std::string list_1_;
  std::string list_2_;

  std::string summary_1_;
  std::string summary_2_;

  std::string path_docx_1_;
  std::string path_docx_2_;

  int amount_spaces_gap_ = 8;

  bool compare_content_ = true;
  std::string path_extract_left_, path_extract_right_;

  int index_total_ = 0;
  int index_left_ = 0;
  int index_right_ = 0;

  static std::vector<std::string> SplitIntoSortedLines(std::string &file_list);

  static bool IsFileItemLine(const std::string &line);

  static std::string RenderMargin(int len_str, int len_max);

  bool AreFilesInLinesDifferent(
      const std::string &line_1, const std::basic_string<char> &line_2);

  static void UpdateColumnStyles(const std::string &line_left,
                                 const std::string &line_right,
                                 std::string &style_on_left,
                                 std::string &style_on_right,
                                 std::string &style_off);

  void AdvanceToAlphabeticalNextItem(const std::string &filename_left,
                                            const std::string &filename_right,
                                            std::string &line_left,
                                            std::string &line_right);
  void OutputHeadline(const std::string &gap,
                      uint32_t len_path_left,
                      u_int32_t len_line_max) const;
};

#endif  // DOCXBOX_DOCX_DOCX_FILELIST_H_
