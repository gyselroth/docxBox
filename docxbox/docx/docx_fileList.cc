// Copyright (c) 2020 gyselroth GmbH

#include <docxbox/docx/docx_fileList.h>
#include <docxbox/helper/helper_string.h>

#include <vector>

void docx_fileList::SortFileListByFilename(std::string &file_list) {
  auto lines = helper::String::Explode(file_list, '\n');

  int line_first_file = 0;
  int line_last_file = 0;

  GetFileListFilesRange(lines, line_first_file, line_last_file);

  auto lines_by_filename = GetVectorOfLinesByFilename(
      lines, line_first_file, line_last_file);

  // Sort tuples by filename
  sort(
      lines_by_filename.begin(),
      lines_by_filename.end(),
      CompareFileListItemTuples);

  // Refill sorted into lines vector
  // (which also has headline, separators, summary)
  int i = 0;

  for (const auto& line : lines_by_filename) {
    lines[line_first_file + i] = std::get<1>(line);

    ++i;
  }

  // Update initially given string reference
  file_list = helper::String::Implode(lines, "\n");
}

std::vector<std::tuple<std::string, std::string>>
docx_fileList::GetVectorOfLinesByFilename(
    const std::vector<std::string> &lines,
    int line_first_file,
    int line_last_file) {
  std::vector<std::tuple<std::string, std::string>> lines_by_filename;

  int i = 0;

  for (const auto& line : lines) {
    if (i>= line_first_file && i <= line_last_file) {
      std::string filename = helper::String::GetTrailingWord(line);

      std::tuple<std::string, std::string> t = std::make_tuple(filename, line);
      lines_by_filename.push_back(t);
    }

    ++i;
  }

  return lines_by_filename;
}

// Comparator method for sorting
bool docx_fileList::CompareFileListItemTuples(
    std::tuple<std::string, std::string> tuple_1,
    std::tuple<std::string, std::string> tuple_2) {
  std::string str_1 = std::get<0>(tuple_1);
  std::string str_2 = std::get<0>(tuple_2);

  return std::strcmp(str_1.c_str(), str_2.c_str()) < 0;
}

// Find lines indexes of 1st and last file item
// (exclude headline, separators, summary)
void docx_fileList::GetFileListFilesRange(
    const std::vector<std::string> &lines,
    int &line_first_file,
    int &line_last_file) {
  int line_number = 0;

  for (auto line : lines) {
    if (line[0]
        && line[0] != '-'
        && helper::String::Contains(line, ".")) helper::String::Trim(line);

    if (line_first_file == 0) {
      if (helper::String::StartsNumeric(line.c_str()))
        line_first_file = line_number;
    } else {
      if (!helper::String::StartsNumeric(line.c_str())) {
        line_last_file = line_number - 1;

        break;
      }
    }

    ++line_number;
  }
}