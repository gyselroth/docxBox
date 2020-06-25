// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_DOCX_XML_CONTENTTYPE_DOCX_XML_FIELD_H_
#define DOCXBOX_DOCX_XML_CONTENTTYPE_DOCX_XML_FIELD_H_

#include <docxbox/app/app_log.h>
#include <docxbox/docx/xml/docx_xml.h>
#include <docxbox/docx/xml/visitor/docx_xml_remove.h>
#include <docxbox/helper/helper_file.h>
#include <docxbox/helper/helper_string.h>

#include <vendor/tinyxml2/tinyxml2.h>

#include <iostream>
#include <string>
#include <vector>

class docx_xml_field: docx_xml {
 public:
  docx_xml_field(int argc, const std::vector<std::string>& argv);

  void CollectFields(const std::string& path_xml);

  bool SetFieldText(const std::string& path_xml,
                    const std::string &field_identifier,
                    const std::string &text);

  void Output(bool as_json);

 private:
  std::vector <std::string> fields_in_current_xml_;

  // Vector of XML filenames that fields have been collected from
  std::vector <std::string> field_xml_files_;
  // Vector of vectors: fields per XML file
  std::vector <std::vector<std::string>> fields_in_xml_files_;

  bool is_inside_searched_field_ = false;

  bool has_xml_changed_ = false;

  void CollectFieldsFromNodes(tinyxml2::XMLElement *node);

  void SetFieldTextInNodes(tinyxml2::XMLElement *node,
                           const std::string &field_identifier,
                           const std::string &field_value);

  // Set field text within children of given node
  // If within merge field: remove its field nodes to transform into text
  void TransformMergeFieldToTextInNodes(tinyxml2::XMLElement *node,
                                        const std::string &field_identifier,
                                        const std::string &field_value);

  // Remove fldChar nodes from merge-field: reduce it into it's text
  static void RemoveFldCharsFromMergeField(tinyxml2::XMLElement *sub_node);

  void OutputAsJson();
};

#endif  // DOCXBOX_DOCX_XML_CONTENTTYPE_DOCX_XML_FIELD_H_
