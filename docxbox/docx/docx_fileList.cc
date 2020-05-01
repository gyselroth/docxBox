// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/docx/docx_fileList.h>

void docx_fileList::SortFileListByFilename(std::string &file_list) {
  auto lines = helper::String::Explode(file_list, '\n');

  int line_first_file = 0;
  int line_last_file = 0;

  GetFileListFilesRange(lines, line_first_file, line_last_file);

  auto lines_by_filename = GetVectorOfLinesByFilename(
      lines, line_first_file, line_last_file);

  // Sort tuples by filename
  sort(lines_by_filename.begin(),
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

bool docx_fileList::IsFinalSeparatorLine(int index, std::string line) {
  return line[0] == '-' && index > 2;
}

// Fill lines of files present in only one list,  with empty lines on the other
void docx_fileList::ParallelizeListItems(
    std::string &list_1, std::string &list_2) {
  auto lines_1 = helper::String::Explode(list_1, '\n');
  auto lines_2 = helper::String::Explode(list_2, '\n');

  unsigned long kAmount_lines_1 = lines_1.size();
  unsigned long kAmount_lines_2 = lines_2.size();

  auto has_more_lines_1 = kAmount_lines_1 > kAmount_lines_2;
  auto has_more_lines_2 = !has_more_lines_1 &&kAmount_lines_1 < kAmount_lines_2;

  if (has_more_lines_1) {
    InsertEmptyLinesAtFilesOnlyInOneLists(lines_1, lines_2, kAmount_lines_1);
  } else if (has_more_lines_2) {
    InsertEmptyLinesAtFilesOnlyInOneLists(lines_2, lines_1, kAmount_lines_2);
  }

  list_1 = helper::String::Implode(lines_1, "\n");
  list_2 = helper::String::Implode(lines_2, "\n");
}

// Iterate over longer list,
// At lines where filenames of both lists are different:
// Insert empty line before alphabetically sorted second filename of both
void docx_fileList::InsertEmptyLinesAtFilesOnlyInOneLists(
    std::vector<std::string> &lines_1,
    std::vector<std::string> &lines_2,
    unsigned long amount_lines) {
  for (int index = 0; index < amount_lines; index++) {
    if (index > amount_lines) break;

    auto line_1 = lines_1[index];

    if (index < 2) {
      ++index;
      continue;
    }

    bool kIs_final_separator_1 = IsFinalSeparatorLine(index, line_1);

    if ((!IsFileItemLine(line_1) && !kIs_final_separator_1)
        || helper::String::IsWhiteSpace(line_1)) {
      ++index;
      continue;
    }

    auto line_2 = lines_2[index];

    bool kIs_final_separator_2 = IsFinalSeparatorLine(index, line_2);

    if (line_1 != line_2
        && !line_2.empty()) {
      auto filename_1 = helper::String::GetTrailingWord(line_1);
      auto filename_2 = helper::String::GetTrailingWord(line_2);

      if (filename_1 != filename_2) {
        // Insert empty line before
        // - alphabetical second filename
        // - final separator of shorter list
        if ((kIs_final_separator_2 && !kIs_final_separator_1)
            || strcmp(filename_1.c_str(), filename_2.c_str()) < 0) {
          lines_2.insert(lines_2.begin() + index - 1, "");
          lines_1.insert(lines_1.end(), "");
        } else {
          lines_1.insert(lines_1.begin() + index - 1, "");
          lines_2.insert(lines_2.end(), "");
        }

        ++amount_lines;
      }
    }

    ++index;
  }
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

  std::string gap;

  if (amount_spaces_gap >= 1) gap = helper::String::Repeat(" ", amount_spaces_gap);

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

    std::string left_margin_right;

    if (len_left < len_left_max) {
      left_margin_right = helper::String::Repeat(" ", len_left_max - len_left);
    }

    std::string right_margin_right;

    if (len_right < len_right_max) {
      right_margin_right =
          helper::String::Repeat(" ", len_right_max - len_right);
    }

    std::string style_on_left, style_on_right, style_off;

    if ((IsFileItemLine(line_left) || IsFileItemLine(line_right))
        && areLinesDifferent(compare_content,
                          path_extract_left,
                          path_extract_right,
                          line_left,
                          line_right)) {
      style_off = kAnsiReset;
      style_on_left = style_on_right = kAnsiReverse;

      if (helper::String::IsWhiteSpace(line_left)) style_on_left = "";
      if (helper::String::IsWhiteSpace(line_right)) style_on_right = "";
    }

    out += style_on_left + line_left.append(left_margin_right).append(style_off)
        .append(gap)
        .append(style_on_right).append(line_right).append(right_margin_right)
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

bool docx_fileList::areLinesDifferent(bool compare_content,
                                      const std::string &path_extract_left,
                                      const std::string &path_extract_right,
                                      const std::string &line_left,
                                      const std::basic_string<char> &line_right) {
  if (line_left != line_right) return true;

  bool kIs_file_left = IsFileItemLine(line_left);
  bool kIs_file_right = IsFileItemLine(line_right);

  if (kIs_file_left != kIs_file_right
      || (!kIs_file_left && !kIs_file_right)) return true;

  if (!compare_content) return false;

  const std::string &filename_left =
      helper::String::GetTrailingWord(line_left);

  const std::string &filename_right =
      helper::String::GetTrailingWord(line_right);

  std::string path_file_left =
      path_extract_left + "/" + filename_left;

  std::string path_file_right =
      path_extract_right + "/" + filename_right;

  if (filename_left == filename_right
      && helper::File::FileExists(path_file_left)
      && helper::File::FileExists(path_file_right)) {
    auto content_left = helper::File::GetFileContents(path_file_left);
    auto content_right = helper::File::GetFileContents(path_file_right);

    return content_left != content_right;
  }

  return false;
}
