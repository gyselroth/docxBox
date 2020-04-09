
#ifndef DOCXBOX_CLASS_XML_REPLACE
#define DOCXBOX_CLASS_XML_REPLACE

#include <string>
#include <iostream>

#include "docx_xml.h"
#include "../../helper/helper_file.h"

#include "../../helper/helper_string.h"
#include "../../../vendor/tinyxml2/tinyxml2.h"

class docx_xml_replace:docx_xml {

 public:
  docx_xml_replace(int argc, char **argv);

  bool ReplaceStringInXml(
      const std::string& path_xml,
      const std::string& search,
      const std::string& replacement,
      bool replace_segmented = false
  );

 private:
  bool replace_segmented_;
  // How many previous (tags containing text) nodes to look behind during current replacement run?
  int segments_look_behind_ = 0;
  std::vector<tinyxml2::XMLElement *> previous_text_nodes_;

  int amount_replaced_ = 0;

  void ReplaceStringInChildNodesText(tinyxml2::XMLElement *node, const std::string& search, const std::string& replacement);
};

#endif //DOCXBOX_CLASS_XML_REPLACE
