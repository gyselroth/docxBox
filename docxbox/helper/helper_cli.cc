// Copyright (c) 2020 gyselroth GmbH

#include <docxbox/helper/helper_cli.h>

namespace helper {

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
