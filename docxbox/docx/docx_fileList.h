// Copyright (c) 2020 gyselroth GmbH

#ifndef DOCXBOX_DOCX_DOCX_FILELIST_H_
#define DOCXBOX_DOCX_DOCX_FILELIST_H_

#include <string>
#include <vector>

class docx_fileList {
 public:
  static void SortFileListByFilename(std::string &file_list);

  // Comparator method for sorting
  static bool CompareFileListItemTuples(
      std::tuple<std::string, std::string> tuple_1,
      std::tuple<std::string, std::string> tuple_2);

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
};

#endif  // #define DOCXBOX_DOCX_DOCX_FILELIST_H_
