// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_DOCX_XML_VISITOR_DOCX_XML_PREPROCESS_H_
#define DOCXBOX_DOCX_XML_VISITOR_DOCX_XML_PREPROCESS_H_

#include <docxbox/docx/xml/docx_xml.h>
#include <docxbox/helper/helper_file.h>
#include <docxbox/helper/helper_string.h>

#include <vendor/tinyxml2/tinyxml2.h>

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

class docx_xml_tidy: public docx_xml  {
 public:
  docx_xml_tidy(int argc, const std::vector<std::string>& argv);

  bool LoadXmlTidy(const std::string& path_xml);
  bool SaveDocToXml(bool decode = true);

  void RemoveDispensableTags();
  static void RemoveDispensableTags(std::string &xml);

  void RestorePreserveSpace();
  static void RestorePreserveSpace(std::string &xml);

  // Defragment XML via iteration over its nodes
  bool DefragmentXml();

  // Locate occurrences of given string within textual nodes,
  // where the string is not the sole content: Split (into up to 3) nodes than
  // to contain the given string alone in a tag of its own.
  // Used as preprocessor before: rmt, rpt
  bool DissectXmlNodesContaining(const std::string& str);

 private:
  std::string path_xml_;

  int run_index_ = 0;
  bool is_within_run_properties_ = false;

  // Vector to collect child-tags of current w:rPr inside
  std::vector<std::string> previous_run_properties_ = {};

  // w:t parent run's style properties (collected child-tags of w:rPr tag)
  std::vector<std::vector<std::string>> properties_of_runs_ = {};

  // Each 1st run of a section (e.g. paragraph)
  // cannot be merged w/ a prev. sibling
  bool is_first_run_of_section = true;

  // found <w:r>s that identically repeat their previous siblings' properties
  bool found_runs_to_be_merged_ = false;

  // <w:t>s that contain a marker-string preceded/followed by other text
  std::vector<tinyxml2::XMLElement*> texts_to_be_split_ = {};

  void InitMarRunsToBeMerged();
  void MarkRunsToBeMerged(tinyxml2::XMLElement *node);
  bool AreLastTwoRunPropertiesIdentical();
  bool MergeNodes();

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

#endif  // DOCXBOX_DOCX_XML_VISITOR_DOCX_XML_PREPROCESS_H_
