// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_DOCX_DOCX_FILELISTCOMPARE_H_
#define DOCXBOX_DOCX_DOCX_FILELISTCOMPARE_H_

#include <docxbox/docx/archive/docx_archive.h>
#include <docxbox/helper/helper_string.h>

#include <algorithm>
#include <string>
#include <tuple>
#include <vector>

// Compare contents (= all files, and their content) from two DOCX archives
class docx_compare {
 public:
  docx_compare(
      std::string list_1, std::string summary_1,
      std::string list_2, std::string summary_2,
      std::string path_docx_1, std::string path_docx_2);

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
  std::string gap_;

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

  void GetCurrentLineAndFilename(int index,
                                 const std::vector<std::string> &lines_left,
                                 uint16_t amount_lines_left,
                                 std::string &line_left,
                                 std::string &filename_left) const;

  void OutputHeadline(uint32_t len_path_left, u_int32_t len_line_max) const;
  void OutputLine(uint32_t len_summary_1, u_int32_t len_line_max) const;

  static int ExtractFileSizeFromLine(const std::string &line);

  void ColorizeFileSize(
      const char *color,
      const std::string &style_on,
      std::string &line) const;
};

#endif  // DOCXBOX_DOCX_DOCX_FILELISTCOMPARE_H_
