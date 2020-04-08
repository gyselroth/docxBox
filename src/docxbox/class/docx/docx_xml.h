
#ifndef DOCXBOX_CLASS_DOCX_WORDPARSER_H
#define DOCXBOX_CLASS_DOCX_WORDPARSER_H

#include <string>
#include <iostream>

#include "../../helper/helper_string.h"

#include "../../../vendor/tinyxml2/tinyxml2.h"

class docx_xml {

 public:
  docx_xml(int argc, char **argv);

  static bool IsXmlFileContainingText(std::string filename);

  std::string GetTextFromXmlFile(std::string path_xml, bool newline_at_segments);

  void Output();

  bool ReplaceStringInXml(std::string path_xml);

 private:
  int argc;
  char **argv;

  std::string document_text;

  void GetChildNodesText(tinyxml2::XMLElement *node, bool newline_at_segments = false);
};

#endif //DOCXBOX_CLASS_DOCX_WORDPARSER_H
