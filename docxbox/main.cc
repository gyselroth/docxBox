// Copyright (c) 2020 gyselroth GmbH

#include <docxbox/app/app.h>
#include <docxbox/helper/helper_dateTime.h>

/**
 * @param argc Amount of arguments
 * @param argv Array of arguments, argv[0] is name and path of executable
 */
int main(int argc, char **argv) {
  // Process command arguments, display help if no valid command given
  auto *app = new docxbox::App(argc, argv);

  bool success = true;

  if (argc > 1) success = app->Process();

  delete app;

  // TODO(kay): return comprehensible bash error codes instead of 125
  return success ? 0 : 125;
}
