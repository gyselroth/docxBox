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
};

#endif  // DOCXBOX_DOCX_DOCX_FILELIST_H_
