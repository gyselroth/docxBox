// Copyright (c) 2020 gyselroth GmbH

#include "docx_xml_fields.h"

docx_xml_fields::docx_xml_fields(int argc, char **argv) : docx_xml(argc, argv) {
}

void docx_xml_fields::CollectMergeFields(const std::string& path_xml, bool as_json) {
  fields_in_current_xml_.clear();

  tinyxml2::XMLDocument doc;

  doc.LoadFile(path_xml.c_str());

  if (doc.ErrorID() != 0) return;

  tinyxml2::XMLElement *body = doc.FirstChildElement("w:document")
      ->FirstChildElement("w:body");

  CollectFieldsFromNodes(body);

  field_xml_files_.push_back(path_xml);
  fields_in_xml_files_.push_back(fields_in_current_xml_);
}

void docx_xml_fields::CollectFieldsFromNodes(tinyxml2::XMLElement *node) {
  if (!node || node->NoChildren()) return;

  tinyxml2::XMLElement *sub_node = node->FirstChildElement();

  if (sub_node == nullptr) return;

  do {
    if (!sub_node) continue;

    const char *value = sub_node->Value();

    if (value) {
      if (0 == strcmp(value, "w:instrText")) {
        fields_in_current_xml_.emplace_back(sub_node->GetText());

        continue;
      } /*else if (0 == strcmp(value, "w:fldChar")) {
        if (
            0 == strcmp(
                sub_node->Attribute("w:fldCharType"),
                "begin"))
      }*/
    }

    CollectFieldsFromNodes(sub_node);
  } while ((sub_node = sub_node->NextSiblingElement()));
}

void docx_xml_fields::Output(bool as_json) {
  int i=0;

  for (const auto& xml_filename : field_xml_files_) {
    std::cout
      << xml_filename << ":\n"
      << helper::String::Repeat("-", xml_filename.length() + 1) << "\n";

      for (const auto& field : fields_in_xml_files_[i]) {
        std::cout << field << "\n";
      }

      std::cout << "\n";

      i++;
  }
}
