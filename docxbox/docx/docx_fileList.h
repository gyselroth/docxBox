// Copyright (c) 2020 gyselroth GmbH

#ifndef DOCXBOX_DOCX_DOCX_FILELIST_H_
#define DOCXBOX_DOCX_DOCX_FILELIST_H_

#include <docxbox/docx/docx_archive.h>
#include <docxbox/helper/helper_string.h>

#include <string>
#include <tuple>
#include <vector>

class docx_fileList {
 public:
  static void SortFileListByFilename(std::string &file_list);

  // Comparator method for sorting
  static bool CompareFileListItemTuples(
      std::tuple<std::string, std::string> tuple_1,
      std::tuple<std::string, std::string> tuple_2);

  // Fill lines of files present in only one list, with empty lines on the other
  static void ParallelizeListItems(std::string &list_1, std::string &list_2);

  static std::string RenderListsCompare(
      const std::string &left,
      const std::string &right,
      int amount_spaces_gap = 8,
      bool compare_content = false,
      const std::string &path_docx_left = "",
      const std::string &path_docx_right = "");

 private:
  // Find lines index of 1st and last file item
  // (exclude headline, separators, summary)
  static void GetFileListFilesRange(const std::vector<std::string> &lines,
                             int &line_first_file,
                             int &line_last_file);

  static std::vector<std::tuple<std::string, std::string>>
  GetVectorOfLinesByFilename(const std::vector<std::string> &lines,
                             int line_first_file,
                             int line_last_file);

  static bool IsFileItemLine(const std::string &line);
  static bool IsFinalSeparatorLine(int index, std::string line);

  static bool areLinesDifferent(bool compare_content,
                                const std::string &path_extract_left,
                                const std::string &path_extract_right,
                                const std::string &line_left,
                                const std::basic_string<char> &line_right);

  // Iterate over longer list,
  // At lines where filenames of both lists are different:
  // Insert empty line before alphabetically sorted second filename of both
  static void InsertEmptyLinesAtFilesOnlyInOneLists(
      std::vector<std::string> &lines_1,
      std::vector<std::string> &lines_2,
      unsigned long amount_lines);
};

#endif  // DOCXBOX_DOCX_DOCX_FILELIST_H_
