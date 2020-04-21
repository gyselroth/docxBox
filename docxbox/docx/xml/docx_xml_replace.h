// Copyright (c) 2020 gyselroth GmbH

#ifndef DOCXBOX_DOCX_DOCX_XML_REPLACE_H_
#define DOCXBOX_DOCX_DOCX_XML_REPLACE_H_

#include <docxbox/docx/xml/docx_xml.h>
#include <docxbox/helper/helper_file.h>
#include <docxbox/helper/helper_string.h>

#include <vendor/tinyxml2/tinyxml2.h>

#include <iostream>
#include <string>
#include <vector>

class docx_xml_replace:docx_xml {
 public:
  docx_xml_replace(int argc, char **argv);

  bool ReplaceStringInXml(
      const std::string& path_xml,
      const std::string& search,
      const std::string& replacement,
      bool replace_segmented = false);

 private:
  bool replace_segmented_;

  // How many previous (tags containing text)
  // nodes to look behind during current replacement run?
  int segments_look_behind_ = 0;

  std::vector<tinyxml2::XMLElement *> previous_text_nodes_;

  int amount_replaced_ = 0;

  void ReplaceStringInTextNodes(tinyxml2::XMLElement *node,
                                const std::string &search,
                                const std::string &replacement);

  void ReplaceSegmentedStringInTextNodes(tinyxml2::XMLElement *node,
                                         const std::string &search,
                                         const std::string &replacement);
};

#endif  // DOCXBOX_DOCX_DOCX_XML_REPLACE_H_
