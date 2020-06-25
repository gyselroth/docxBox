// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/docx/docx_diff.h>

void docx_diff::OutputSideBySide(const std::string &path_extract_left,
                                 const std::string &path_extract_right,
                                 const std::string &file) {
  auto path_left = path_extract_left;
  helper::String::Replace(&path_left, "-extracted", "");

  auto path_right = path_extract_right;
  helper::String::Replace(&path_right, "-extracted", "");

  std::cout
    << file << " of "
    << helper::Cli::ANSI_UNDERLINE << path_left << helper::Cli::ANSI_RESET
    << " (left) vs. "
    << helper::Cli::ANSI_UNDERLINE << path_right << helper::Cli::ANSI_RESET
    << "\n\n";

  int width = helper::File::GetLongestLineLength(
  path_extract_left + "/" + file,
  path_extract_right + "/" + file);

  // TODO(kay): test/improve auto-width
  if (width > 200) width = 200;

  helper::Cli::Execute(
      std::string("diff -y "
                  "--width=" + std::to_string(width) + " "
                  + path_extract_left + "/" + file + " "
                  + path_extract_right + "/" + file).c_str());
}

void docx_diff::OutputUnified(const std::string &path_extract_left,
                              const std::string &path_extract_right,
                              const std::string &file) {
  auto output = helper::Cli::GetExecutionResponse(
      std::string(
          "diff -u "
          + path_extract_left + "/" + file + " "
          + path_extract_right + "/" + file).c_str());

  helper::String::Replace(&output, "-extracted/", "");

  std::string output_colored;

  auto lines = helper::String::Explode(output, '\n');

  for (auto line : lines) {
    if (line[0] == '-')
      (line += helper::Cli::ANSI_RESET).insert(0, helper::Cli::ANSI_LIGHT_RED);
    else if (line[0] == '+')
      (line += helper::Cli::ANSI_RESET)
      .insert(0, helper::Cli::ANSI_LIGHT_GREEN);
    else if (line[0] == '@')
      (line += helper::Cli::ANSI_RESET).insert(0, helper::Cli::ANSI_BOLD);

    output_colored += line + "\n";
  }

  std::cout << output_colored;
}
