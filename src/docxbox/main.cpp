
#include "class/app/app.h"

/**
 * @param argc Amount of arguments received
 * @param argv Array of arguments received, argv[0] is name and path of executable
 */
int main(int argc, char **argv) {
  //argc = 3;
  //argv[1] = "txt";
  //argv[2] = "/home/kay/CLionProjects/docxengine/test/files/file-sample_100kB.docx";

  // Process command arguments, display help if no valid command given
  auto *app = new docxbox::App(argc, argv);

  if (argc > 1) app->Process();

  delete app;

  return 0;
}
