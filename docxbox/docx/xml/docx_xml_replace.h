// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_DOCX_XML_DOCX_XML_REPLACE_H_
#define DOCXBOX_DOCX_XML_DOCX_XML_REPLACE_H_

#include <docxbox/docx/renderer/docx_renderer_delegate.h>
#include <docxbox/docx/docx_media.h>
#include <docxbox/docx/xml/docx_xml.h>
#include <docxbox/helper/helper_file.h>
#include <docxbox/helper/helper_string.h>

#include <vendor/tinyxml2/tinyxml2.h>

#include <iostream>
#include <string>
#include <vector>

class docx_xml_replace:docx_xml, public docx_renderer_delegate  {
 public:
  docx_xml_replace(int argc, char **argv);

  bool ReplaceInXml(
      const std::string& path_xml,
      const std::string& search,
      const std::string& replacement,
      bool replace_segmented = false);

  void SetImageRelationshipId(std::string &relationship_id);

  void SetReplacementXmlFirstChildTag(
      const std::string &replacement_xml_first_child_tag);

 private:
  // Replacement can be (plain)textual or XML markup
  bool is_replacement_xml_ = false;

  bool replace_segmented_;

  // How many previous (tags containing text)
  // nodes to look behind during current replacement run?
  int segments_look_behind_ = 0;

  // When replacing text by XML, the resp. parent nodes
  // (having child-nodes containing the text) must be replaced
  std::vector<tinyxml2::XMLElement *> runs_to_be_replaced_;

  tinyxml2::XMLElement *current_run_;

  tinyxml2::XMLElement *replacement_xml_element_;

  // Nodes stack, for when replacing text that is contained segmented
  std::vector<tinyxml2::XMLElement *> previous_text_nodes_;

  int amount_replaced_ = 0;

  void ReplaceOrLocateStringInXml(tinyxml2::XMLElement *node,
                                  const std::string &search,
                                  const std::string &replacement);

  void ReplaceSegmentedStringInTextNodes(tinyxml2::XMLElement *node,
                                         const std::string &search,
                                         const std::string &replacement);

  void ReplaceRunsByXmlElement();
};

#endif  // DOCXBOX_DOCX_XML_DOCX_XML_REPLACE_H_
