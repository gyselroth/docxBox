// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/helper/helper_cli.h>

namespace helper {

const char Cli::ANSI_BOLD[] = "\033[1m";
const char Cli::ANSI_DIM[] = "\033[2m";
const char Cli::ANSI_UNDERLINE[] = "\033[4m";
const char Cli::ANSI_REVERSE[] = "\033[7m";
const char Cli::ANSI_RESET[] = "\033[0m";

const char Cli::ANSI_LIGHT_GREEN[] = "\033[1;32m";
const char Cli::ANSI_LIGHT_RED[] = "\033[1;31m";

void Cli::Execute(const char *command) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
  system(command);
#pragma clang diagnostic pop
}

std::string Cli::GetExecutionResponse(const char *command) {
  FILE *fp;
  char path[1035];

  // Open the command for reading
  fp = popen(command, "r");
  if (fp ==nullptr) return "Failed to run command";

  // Read the output a line at a time
  std::string result;
  while (fgets(path, sizeof(path), fp) !=nullptr) {
    result += path;
  }

  pclose(fp);

  return result;
}

}  // namespace helper
