// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_DOCX_XML_VISITOR_DOCX_XML_DISSECT_H_
#define DOCXBOX_DOCX_XML_VISITOR_DOCX_XML_DISSECT_H_

#include <docxbox/docx/xml/docx_xml.h>
#include <docxbox/helper/helper_file.h>
#include <docxbox/helper/helper_string.h>

#include <vendor/tinyxml2/tinyxml2.h>

#include <iostream>
#include <string>
#include <vector>

class docx_xml_dissect:docx_xml  {
 public:
  docx_xml_dissect(int argc, const std::vector<std::string>& argv);

  bool LoadXml(const std::string& path_xml);
  bool SaveXml();

  // Locate occurrences of given string within textual nodes,
  // where the string is not the sole content: Split (into up to 3) nodes than
  // to contain the given string alone in a tag of its own.
  // Used as preprocessor before: rmt, rpt
  bool DissectXmlNodesContaining(const std::string& str);

 private:
  std::string path_xml_;
  tinyxml2::XMLDocument doc_;

  std::vector<tinyxml2::XMLElement*> nodes_to_be_split_ = {};

  // w:t parent run's style properties (w:rPr tag and its child-tags)
  std::vector<std::vector<std::string>> nodes_run_properties_ = {};

  bool is_within_run_properties_ = false;
  std::vector<std::string> previous_run_properties_ = {};

  void LocateNodesContaining(tinyxml2::XMLElement *node,
                             const std::string &str);

  bool SplitNodes(const char *str);

  std::string RenderSplitAfter(const char *str,
                               const std::string &node_text,
                               int index) const;

  std::string RenderSplitBefore(const char *str,
                                const std::string &node_text,
                                int index) const;

  std::string RenderSplitAround(const char *str,
                                const std::string &node_text,
                                int index) const;

  std::string RenderRunProperties(int index) const;

  void DecodeXmlEntities();
};

#endif  // DOCXBOX_DOCX_XML_VISITOR_DOCX_XML_DISSECT_H_
