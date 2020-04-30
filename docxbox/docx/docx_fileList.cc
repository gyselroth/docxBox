// Copyright (c) 2020 gyselroth GmbH

#include <docxbox/docx/docx_fileList.h>

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
    if (IsFileItemLine(line)) helper::String::Trim(line);

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

bool docx_fileList::IsFileItemLine(const std::string &line) {
  return line[0]
          && line[0] != '-'
          && helper::String::Contains(line, ".");
}

std::string docx_fileList::RenderListsCompare(
    const std::string &left,
    const std::string &right,
    int amount_spaces_gap,
    bool compare_content,
    const std::string &path_docx_left,
    const std::string &path_docx_right) {
  auto lines_left = helper::String::Explode(left, '\n');
  auto lines_right = helper::String::Explode(right, '\n');

  uint32_t amount_lines_right = lines_right.size();

  int index = 0;

  std::string gap = amount_spaces_gap < 1
    ? ""
    : helper::String::Repeat(" ", amount_spaces_gap);

  std::string out;

  int len_left_max = helper::String::GetMaxLength(lines_left);
  int len_right_max = helper::String::GetMaxLength(lines_right);

  if (compare_content && (path_docx_left.empty() || path_docx_right.empty()))
    compare_content = false;

  std::string path_extract_left, path_extract_right;

  if (compare_content) {
    path_extract_left =
        docx_archive::UnzipDocx(path_docx_left, "", "cmp_left_");

    path_extract_right =
        docx_archive::UnzipDocx(path_docx_right, "", "cmp_right_");
  }

  for (auto line_left : lines_left) {
    auto line_right = amount_lines_right > index ? lines_right[index] : "";

    uint32_t len_left = line_left.length();
    uint32_t len_right = line_right.length();

    std::string left_margin_right = len_left < len_left_max
      ? helper::String::Repeat(" ", len_left_max - len_left)
      : "";

    std::string right_margin_right = len_right < len_right_max
      ? helper::String::Repeat(" ", len_right_max - len_right)
      : "";

    std::string style_on, style_off;

    bool are_lines_different = false;

    if (line_left != line_right) {
      are_lines_different = false;
    } else {
      if (compare_content && IsFileItemLine(line_left)) {
        std::string path_file_left =
            path_extract_left + "/"
            + helper::String::GetTrailingWord(line_left);

        std::string path_file_right =
            path_extract_right + "/"
            + helper::String::GetTrailingWord(line_right);

        if (helper::File::FileExists(path_file_left)) {
          auto content_left = helper::File::GetFileContents(path_file_left);
          auto content_right = helper::File::GetFileContents(path_file_right);

          are_lines_different = content_left != content_right;
        }
      }
    }

    if (are_lines_different) {
      style_on = kAnsiReverse;
      style_off = kAnsiReset;
    }

    out += style_on + line_left.append(left_margin_right).append(style_off)
        .append(gap)
        .append(style_on).append(line_right).append(right_margin_right)
        .append(style_off)
        .append("\n");

    ++index;
  }

  if (compare_content) {
    helper::File::RemoveRecursive(path_extract_left.c_str());
    helper::File::RemoveRecursive(path_extract_right.c_str());
  }

  return out;
}
