
#ifndef DOCXBOX_CLASS_DOCX_WORDPARSER_H
#define DOCXBOX_CLASS_DOCX_WORDPARSER_H

#include <string>
#include <iostream>

#include "../../helper/helper_file.h"
#include "../../helper/helper_string.h"

#include "../../../vendor/tinyxml2/tinyxml2.h"

class docx_xml {

 public:
  docx_xml(int argc, char **argv);

  static bool IsXmlFileContainingText(std::string filename);

  std::string GetTextFromXmlFile(std::string path_xml, bool newline_at_segments = false);

  void Output();

  bool ReplaceStringInXml(const std::string& path_xml, std::string search, std::string replacement);

 private:
  int argc;
  char **argv;

  std::string document_text;

  int amount_replaced = 0;

  void GetChildNodesText(tinyxml2::XMLElement *node, bool newline_at_segments = false);

  void ReplaceStringInChildNodesText(tinyxml2::XMLElement *node, const std::string& search, const std::string& replacement);
};

#endif //DOCXBOX_CLASS_DOCX_WORDPARSER_H
