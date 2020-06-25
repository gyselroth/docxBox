// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/docx/docx_compare.h>

#include <utility>

docx_compare::docx_compare(std::string list_1, std::string summary_1,
                           std::string list_2, std::string summary_2,
                           std::string path_docx_1, std::string path_docx_2) {
  list_1_ = std::move(list_1);
  list_2_ = std::move(list_2);

  summary_1_ = std::move(summary_1);
  summary_2_ = std::move(summary_2);

  path_docx_1_ = std::move(path_docx_1);
  path_docx_2_ = std::move(path_docx_2);

  if (amount_spaces_gap_ >= 1)
    gap_ = helper::String::Repeat(" ", amount_spaces_gap_);

  if (compare_content_) {
    auto *archive = new docx_archive(0, {}, false);

    path_extract_left_ = archive->UnzipDocx(path_docx_1_, "", "cmp_left_");
    path_extract_right_ = archive->UnzipDocx(path_docx_2_, "", "cmp_right_");

    delete archive;
  }
}

void docx_compare::Output() {
  auto lines_left = SplitIntoSortedLines(list_1_);
  auto lines_right = SplitIntoSortedLines(list_2_);

  auto amount_lines_left = lines_left.size();
  auto amount_lines_right = lines_right.size();
  auto amount_lines_total = amount_lines_left + amount_lines_right;

  std::string line_left, line_right, filename_left, filename_right;
  std::string style_on_left, style_on_right, style_off;

  // Get max. line lengths
  uint32_t len_path_left = path_docx_1_.length() + 1;
  uint32_t len_path_right = path_docx_2_.length() + 1;

  uint32_t len_summary_1 = summary_1_.length();
  uint32_t len_summary_right = summary_2_.length();

  auto len_line_max = helper::String::GetMaxLineLength(lines_left, lines_right);

  if (len_line_max < len_path_left) len_line_max = len_path_left;
  if (len_line_max < len_path_right) len_line_max = len_path_right;
  if (len_line_max < len_summary_1) len_line_max = len_summary_1;
  if (len_line_max < len_summary_right) len_line_max = len_summary_right;

  OutputHeadline(len_path_left, len_line_max);

  // Render file item lines
  while (index_total_ < amount_lines_total) {
    GetCurrentLineAndFilename(
        index_left_, lines_left, amount_lines_left,
        &line_left, &filename_left);

    GetCurrentLineAndFilename(
        index_right_, lines_right, amount_lines_right,
        &line_right, &filename_right);

    AdvanceToAlphabeticalNextItem(
        filename_left, filename_right, &line_left, &line_right);

    auto len_left = line_left.length();
    auto len_right = line_right.length();

    if (0 == len_left && 0 == len_right) continue;

    style_on_left = style_on_right = "";

    if ((IsFileItemLine(line_left) || IsFileItemLine(line_right))
        && AreFilesInLinesDifferent(&line_left, &line_right))
      UpdateColumnStyles(line_left,
                         line_right,
                         &style_on_left,
                         &style_on_right,
                         &style_off);

    if (style_on_left == style_on_right
        && !line_left.empty() && !line_right.empty()) {
      auto file_size_left = ExtractFileSizeFromLine(line_left);
      auto file_size_right = ExtractFileSizeFromLine(line_right);

      if (file_size_left > file_size_right) {
        ColorizeFileSize(
            helper::Cli::ANSI_LIGHT_GREEN, style_on_left, &line_left);
        ColorizeFileSize(
            helper::Cli::ANSI_LIGHT_RED, style_on_right, &line_right);
      } else if (file_size_left < file_size_right) {
        ColorizeFileSize(
            helper::Cli::ANSI_LIGHT_GREEN, style_on_right, &line_right);
        ColorizeFileSize(
            helper::Cli::ANSI_LIGHT_GREEN, style_on_left, &line_left);
      }
    }

    std::cout << style_on_left + line_left
              << RenderMargin(len_left, len_line_max)
              << style_off
              << gap_
              << style_on_right + line_right
              << RenderMargin(len_right, len_line_max)
              << style_off + "\n";
  }

  OutputLine(len_summary_1, len_line_max);
}

void docx_compare::ColorizeFileSize(const std::string& color,
                                    const std::string &style_on,
                                    std::string *line) const {
  *line = color + (*line).insert(9, helper::Cli::ANSI_RESET + style_on);
}

void docx_compare::OutputLine(uint32_t len_summary_1,
                              u_int32_t len_line_max) const {
  std::cout << "---------                     -------"
            << helper::String::Repeat("-", len_line_max - 37)
            << gap_
            << "---------                     -------"
            << helper::String::Repeat("-", len_line_max - 37) << "\n"
            << summary_1_
            << RenderMargin(len_summary_1, len_line_max)
            << gap_
            << summary_2_ + "\n";
}

void docx_compare::GetCurrentLineAndFilename(
    int index,
    const std::vector<std::string> &lines,
    uint16_t amount_lines,
    std::string *line,
    std::string *filename) const {
  if (index < amount_lines) {
    *line = lines[index];
    *filename = helper::String::GetTrailingWord(line);
  } else {
    *line = "";
    *filename = "";
  }
}

void docx_compare::OutputHeadline(uint32_t len_path_left,
                                  u_int32_t len_line_max) const {
  std::cout << "\n"
            << path_docx_1_ << ":"
            << RenderMargin(len_path_left, len_line_max)
            << gap_ << path_docx_2_ << ":\n\n"
            << "   Length        Date  Time   Name"
            << RenderMargin(34, len_line_max)
            << gap_
            << "   Length        Date  Time   Name\n"
            << "---------  ---------- -----   ----"
            << helper::String::Repeat("-", len_line_max - 34)
            << gap_
            << "---------  ---------- -----   ----"
            << helper::String::Repeat("-", len_line_max - 34)
            << "\n";
}

std::vector<std::string> docx_compare::SplitIntoSortedLines(
    const std::string &file_list) {
  auto lines = helper::String::Explode(file_list, '\n');

  // Sort tuples alphabetic by filename (trailing item within each line)
  sort(lines.begin(),
       lines.end(),
       CompareLinesByFilenames);

  return lines;
}

// Comparator method for sorting
bool docx_compare::CompareLinesByFilenames(std::string str_1,
                                           std::string str_2) {
  auto filename_1 = helper::String::GetTrailingWord(&str_1);
  auto filename_2 = helper::String::GetTrailingWord(&str_2);

  return std::strcmp(filename_1.c_str(), filename_2.c_str()) < 0;
}

bool docx_compare::IsFileItemLine(const std::string &line) {
  return line[0]
      && line[0] != '-'
      && helper::String::Contains(line, ".");
}

void docx_compare::AdvanceToAlphabeticalNextItem(
    const std::string &filename_left,
    const std::string &filename_right,
    std::string *line_left,
    std::string *line_right) {
  // Compare to find alphabetical order
  int comparator = strcmp(filename_left.c_str(), filename_right.c_str());

  ++index_total_;

  if (comparator < 0) {
    // left < right  -> output left, empty on right
    *line_right = "";

    ++index_left_;
  } else if (comparator == 0) {
    // left == right -> output both
    ++index_left_;
    ++index_right_;
    ++index_total_;
  } else {
    // left > right -> empty on left, output right
    *line_left = "";

    ++index_right_;
  }
}

void docx_compare::UpdateColumnStyles(const std::string &line_left,
                                      const std::string &line_right,
                                      std::string *style_on_left,
                                      std::string *style_on_right,
                                      std::string *style_off) {
  *style_off = helper::Cli::ANSI_RESET;

  *style_on_left = helper::Cli::ANSI_REVERSE;
  *style_on_right = helper::Cli::ANSI_REVERSE;

  bool is_blank_left = helper::String::IsWhiteSpace(line_left);
  bool is_blank_right = helper::String::IsWhiteSpace(line_right);

  if (is_blank_left) {
    if (!is_blank_right) {
      *style_on_right += helper::Cli::ANSI_DIM;
      *style_on_right += helper::Cli::ANSI_LIGHT_GREEN;

      *style_on_left += helper::Cli::ANSI_DIM;
      *style_on_left += helper::Cli::ANSI_LIGHT_RED;
    } else {
      *style_on_left = "";
    }
  }

  if (is_blank_right) {
    if (!is_blank_left) {
      *style_on_left += std::string(helper::Cli::ANSI_DIM)
          + std::string(helper::Cli::ANSI_LIGHT_GREEN);

      *style_on_right += std::string(helper::Cli::ANSI_DIM)
          + std::string(helper::Cli::ANSI_LIGHT_RED);
    } else {
      *style_on_right = "";
    }
  }
}

int docx_compare::ExtractFileSizeFromLine(const std::string &line) {
  auto line_tmp = line;
  helper::String::LTrim(&line_tmp);

  auto parts = helper::String::Explode(line_tmp, ' ');

  if (parts.empty()) return 0;

  return helper::String::IsNumeric(&parts[0], false, false, false)
         ? std::stoi(parts[0])
         : 0;
}

std::string docx_compare::RenderMargin(int len_str, int len_max) {
  return len_str < len_max
    ? helper::String::Repeat(" ", len_max - len_str)
    : "";
}

bool docx_compare::AreFilesInLinesDifferent(
    std::string *line_1, std::string *line_2) {
  if (line_1 != line_2) return true;

  bool kIs_file_left = IsFileItemLine(*line_1);
  bool kIs_file_right = IsFileItemLine(*line_2);

  if (kIs_file_left != kIs_file_right
      || (!kIs_file_left && !kIs_file_right)) return true;

  if (!compare_content_) return false;

  const std::string &filename_left =
      helper::String::GetTrailingWord(line_1);

  const std::string &filename_right =
      helper::String::GetTrailingWord(line_2);

  std::string path_file_left = path_extract_left_ + "/" + filename_left;
  std::string path_file_right = path_extract_right_ + "/" + filename_right;

  if (filename_left != filename_right
      || !helper::File::FileExists(path_file_left)
      || !helper::File::FileExists(path_file_right)) return false;

    auto content_left = helper::File::GetFileContents(path_file_left);
    auto content_right = helper::File::GetFileContents(path_file_right);

    return content_left != content_right;
}
