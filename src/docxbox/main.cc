
#include "class/app/app.h"

/**
 * @param argc Amount of arguments received
 * @param argv Array of arguments received, argv[0] is name and path of executable
 */
int main(int argc, char **argv) {
  // Process command arguments, display help if no valid command given
  auto *app = new docxbox::App(argc, argv);

  bool success = true;

  if (argc > 1) success = app->Process();

  delete app;

  // TODO return comprehensible bash error codes instead of 125 (="operation cancelled")
  return success ? 0 : 125;
}
