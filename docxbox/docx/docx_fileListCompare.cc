// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/docx/docx_fileListCompare.h>

#include <utility>

std::vector<std::string> docx_fileListCompare::SplitIntoSortedLines(
    std::string &file_list) {
  auto lines = helper::String::Explode(file_list, '\n');

  // Sort tuples alphabetic by filename (trailing item within each line)
  sort(lines.begin(),
       lines.end(),
       CompareLinesByFilenames);

  return lines;
}

// Comparator method for sorting
bool docx_fileListCompare::CompareLinesByFilenames(
    std::string str_1, std::string str_2) {
  auto filename_1 = helper::String::GetTrailingWord(std::move(str_1));
  auto filename_2 = helper::String::GetTrailingWord(std::move(str_2));

  return std::strcmp(filename_1.c_str(), filename_2.c_str()) < 0;
}

bool docx_fileListCompare::IsFileItemLine(const std::string &line) {
  return line[0]
          && line[0] != '-'
          && helper::String::Contains(line, ".");
}

std::string docx_fileListCompare::RenderListsComparison(
    std::string list_1,
    const std::string& summary_1,
    std::string list_2,
    const std::string& summary_2,
    int amount_spaces_gap,
    bool compare_content,
    const std::string &path_docx_left,
    const std::string &path_docx_right) {
  std::string gap;

  auto *archive = new docx_archive(0, nullptr);

  std::string path_extract_left, path_extract_right;

  if (compare_content) {
    path_extract_left = archive->UnzipDocx(path_docx_left, "", "cmp_left_");
    path_extract_right = archive->UnzipDocx(path_docx_right, "", "cmp_right_");
  }

  if (amount_spaces_gap >= 1)
    gap = helper::String::Repeat(" ", amount_spaces_gap);

  auto lines_left = SplitIntoSortedLines(list_1);
  auto lines_right = SplitIntoSortedLines(list_2);

  auto amount_lines_left = lines_left.size();
  auto amount_lines_right = lines_right.size();

  auto amount_lines_total = amount_lines_left + amount_lines_right;

  int index_total = 0;
  int index_left = 0;
  int index_right = 0;

  std::string line_left, line_right, filename_left, filename_right;
  std::string style_on_left, style_on_right, style_off;

  std::string out;

  // Get max. line lengths
  uint32_t len_path_left = path_docx_left.length() + 1;
  uint32_t len_path_right = path_docx_right.length() + 1;

  uint32_t len_summary_1 = summary_1.length();
  uint32_t len_summary_right = summary_2.length();

  auto len_line_max = helper::String::GetMaxLineLength(lines_left, lines_right);

  if (len_line_max < len_path_left) len_line_max = len_path_left;
  if (len_line_max < len_path_right) len_line_max = len_path_right;
  if (len_line_max < len_summary_1) len_line_max = len_summary_1;
  if (len_line_max < len_summary_right) len_line_max = len_summary_right;

  // Render headline
  out = "\n"
      + path_docx_left + ":";
  out += RenderMargin(len_path_left, len_line_max);
  out += gap + path_docx_right + ":\n\n";

  out += "   Length        Date  Time   Name";
  out += RenderMargin(34, len_line_max);
  out += gap + "   Length        Date  Time   Name\n";

  out += "---------  ---------- -----   ----";
  out += helper::String::Repeat("-", len_line_max - 34);
  out += gap;
  out += "---------  ---------- -----   ----";
  out += helper::String::Repeat("-", len_line_max - 34);
  out += "\n";

  // Render file item lines
  while (index_total < amount_lines_total) {
    if (index_left < amount_lines_left) {
      line_left = lines_left[index_left];
      filename_left = helper::String::GetTrailingWord(line_left);
    } else {
      line_left = "";
      filename_left = "";
    }

    if (index_right < amount_lines_right) {
      line_right = lines_right[index_right];
      filename_right = helper::String::GetTrailingWord(line_right);
    } else {
      line_right = "";
      filename_right = "";
    }

    AdvanceToAlphabeticalNextItem(
        filename_left,
        filename_right,
        index_total,
        index_left,
        index_right,
        line_left,
        line_right);

    auto len_left = line_left.length();
    auto len_right = line_right.length();

    if (0 == len_left && 0 == len_right) continue;

    style_on_left = style_on_right = "";

    if ((IsFileItemLine(line_left) || IsFileItemLine(line_right))
        && AreFileInLinesDifferent(compare_content,
                                   path_extract_left,
                                   path_extract_right,
                                   line_left,
                                   line_right)) {
      UpdateColumnStyles(line_left,
                         line_right,
                         style_on_left,
                         style_on_right,
                         style_off);

    }

    out += style_on_left + line_left;
    out += RenderMargin(len_left, len_line_max);
    out += style_off;

    out += gap;

    out += style_on_right + line_right;
    out += RenderMargin(len_right, len_line_max);
    out += style_off + "\n";

    ++index_total;
  }

  out += "---------                     -------"
      + helper::String::Repeat("-", len_line_max - 37)
      + gap
      + "---------                     -------"
      + helper::String::Repeat("-", len_line_max - 37) + "\n";

  out += summary_1
      + RenderMargin(len_summary_1, len_line_max)
      + gap
      + summary_2 + "\n";

  delete archive;

  return out;
}

void docx_fileListCompare::AdvanceToAlphabeticalNextItem(
    const std::string &filename_left,
    const std::string &filename_right,
    int &index_total,
    int &index_left,
    int &index_right,
    std::string &line_left,
    std::string &line_right) {
  // Compare to find alphabetical order
  int comparator = strcmp(filename_left.c_str(), filename_right.c_str());

  if (comparator < 0) {
    // left < right  -> output left, empty on right
    line_right = "";

    ++index_left;
    ++index_total;
  } else if (comparator == 0) {
    // left == right -> output both
    ++index_left;
    ++index_right;
    index_total += 2;
  } else {
    // left > right -> empty on left, output right
    line_left = "";

    ++index_right;
    ++index_total;
  }
}

void docx_fileListCompare::UpdateColumnStyles(const std::string &line_left,
                                              const std::string &line_right,
                                              std::string &style_on_left,
                                              std::string &style_on_right,
                                              std::string &style_off) {
  style_off = kAnsiReset;

  style_on_left = style_on_right = kAnsiReverse;

  bool is_blank_left = helper::String::IsWhiteSpace(line_left);
  bool is_blank_right = helper::String::IsWhiteSpace(line_right);

  if (is_blank_left) {
    style_on_left = "";

    if (!is_blank_right) style_on_right += kAnsiDim;
  }

  if (is_blank_right) {
    style_on_right = "";

    if (!is_blank_left) style_on_left += kAnsiDim;
  }
}

std::string docx_fileListCompare::RenderMargin(int len_str, int len_max) {
  return len_str < len_max
    ? helper::String::Repeat(" ", len_max - len_str)
    : "";
}

bool docx_fileListCompare::AreFileInLinesDifferent(
    bool compare_content,
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
