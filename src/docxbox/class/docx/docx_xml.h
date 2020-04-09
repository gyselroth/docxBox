
#ifndef DOCXBOX_CLASS_DOCX_XML
#define DOCXBOX_CLASS_DOCX_XML

#include <string>
#include <iostream>

#include "../../helper/helper_file.h"
#include "../../helper/helper_string.h"

#include "../../../vendor/tinyxml2/tinyxml2.h"

class docx_xml {

 public:
  docx_xml(int argc, char **argv);

  static bool IsXmlFileContainingText(std::string filename);

  bool ReplaceStringInXml(const std::string& path_xml, std::string search, std::string replacement);

  void Output();

 protected:
  int argc;
  char **argv;

  std::string document_text;

 private:
  int amount_replaced = 0;

  void ReplaceStringInChildNodesText(tinyxml2::XMLElement *node, const std::string& search, const std::string& replacement);
};

#endif //DOCXBOX_CLASS_DOCX_XML
