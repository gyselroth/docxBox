// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/app/app.h>
#include <docxbox/helper/helper_dateTime.h>

/**
 * @param argc Amount of arguments
 * @param argv Array of arguments, argv[0] is name and path of executable
 */
int main(int argc, char **argv) {
//  std::cout << "debug is active: no dynamic arguments will be read!\n";
//  argc=4;
//  argv[1]="batch";
//  argv[2]="/home/kay/CLionProjects/docxengine/test/files/docx/ohne_bild.docx";
//  argv[3]="{"
//            "\"1\":{"
//              "\"rpt1\":"
//              "["
//                "\"foo\","
//                "{\"h1\": {\"text\": \"Heading\",\"post\": {\"para\": \"my-postfix\"}}}"
//              "]"
//            "},"
//            "\"2\":{"
//              "\"rpt2\":"
//              "["
//                "\"my-postfix\","
//                "{\"h2\": {\"text\": \"Sub-Heading\"}}"
//              "]"
//            "}"
//          "}";

  // Process command arguments, display help if no valid command given
  auto *app = new docxbox::App(argc, argv);

  bool success = argc > 1 ? app->Process() : false;

  delete app;

  // TODO(kay): return comprehensible bash error codes instead of 125
  return success ? 0 : 125;
}
