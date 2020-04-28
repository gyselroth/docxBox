// Copyright (c) 2020 gyselroth GmbH

#include <docxbox/docx/xml/docx_xml_fields.h>

docx_xml_fields::docx_xml_fields(int argc, char **argv) : docx_xml(argc, argv) {
}

void docx_xml_fields::CollectMergeFields(const std::string& path_xml) {
  fields_in_current_xml_.clear();

  tinyxml2::XMLDocument doc;

  doc.LoadFile(path_xml.c_str());

  if (doc.ErrorID() != 0) return;

  tinyxml2::XMLElement *body =
      doc.FirstChildElement("w:document")->FirstChildElement("w:body");

  CollectFieldsFromNodes(body);

  field_xml_files_.push_back(path_xml);
  fields_in_xml_files_.push_back(fields_in_current_xml_);
}

void docx_xml_fields::CollectFieldsFromNodes(tinyxml2::XMLElement *node) {
  if (!node || node->NoChildren()) return;

  tinyxml2::XMLElement *sub_node = node->FirstChildElement();

  if (sub_node == nullptr) return;

  do {
    const char *value = sub_node->Value();

    if (value && 0 == strcmp(value, "w:instrText"))
        fields_in_current_xml_.emplace_back(sub_node->GetText());

    CollectFieldsFromNodes(sub_node);
  } while ((sub_node = sub_node->NextSiblingElement()));
}

bool docx_xml_fields::SetFieldValue(
    const std::string& path_xml,
    const std::string &field_identifier,
    const std::string &value) {
  tinyxml2::XMLDocument doc;
  doc.LoadFile(path_xml.c_str());

  if (doc.ErrorID() != 0) return false;

  tinyxml2::XMLElement *body =
      doc.FirstChildElement("w:document")->FirstChildElement("w:body");

  is_inside_searched_field_ = false;

  SetFieldTextAndCollectFieldNodes(body, field_identifier, value);

  if (!nodes_to_be_removed_.empty()) {
    previous_run_ = nullptr;

    RemoveNodes(nodes_to_be_removed_);

    if (tinyxml2::XML_SUCCESS != doc.SaveFile(path_xml.c_str(), true)) {
      std::cerr << "Error - Failed saving: " << path_xml << "\n";

      return false;
    }
  }

  return true;
}

void docx_xml_fields::SetFieldTextAndCollectFieldNodes(
    tinyxml2::XMLElement *node,
    const std::string &field_identifier,
    const std::string &field_value) {
  if (!node || node->NoChildren()) return;

  tinyxml2::XMLElement *sub_node = node->FirstChildElement();

  if (sub_node == nullptr) return;

  do {
    const char *value = sub_node->Value();

    if (value) {
      if (0 == strcmp(value, "w:r")) {
        previous_run_ = sub_node;
      } else if (0 == strcmp(value, "w:fldChar")) {
        std::string type = sub_node->Attribute("w:fldCharType");

        if (type == "begin") {
          //previous_field_begin_ = sub_node;
          run_around_field_begin_ = previous_run_;
        } else if (type == "end") {
          //previous_field_end_ = sub_node;
          run_around_field_end_ = previous_run_;
        }
      } else if (0 == strcmp(value, "w:instrText")) {
        std::string iterated_field_identifier = sub_node->GetText();
        helper::String::Trim(iterated_field_identifier);

        if (helper::String::StartsWith(
            iterated_field_identifier.c_str(),
            field_identifier.c_str())) {
          run_around_field_instrText_ = previous_run_;
          is_inside_searched_field_ = true;
        }
      } else if (is_inside_searched_field_ && 0 == strcmp(value, "w:t")) {
        sub_node->SetText(field_value.c_str());
      }
    }

    if (is_inside_searched_field_) {
      // Merge field found, text has been set.
      // Mark field nodes to be removed to reduce field to textual component
      nodes_to_be_removed_.push_back(run_around_field_begin_);
      nodes_to_be_removed_.push_back(run_around_field_instrText_);
      nodes_to_be_removed_.push_back(run_around_field_end_);

      is_inside_searched_field_ = false;
    }

    SetFieldTextAndCollectFieldNodes(sub_node, field_identifier, field_value);
  } while ((sub_node = sub_node->NextSiblingElement()));
}

void docx_xml_fields::Output(bool as_json) {
  if (as_json) return OutputAsJson();

  int i = 0;

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

void docx_xml_fields::OutputAsJson() {
  std::cout << "{";

  int i = 0;

  for (const auto& xml_filename : field_xml_files_) {
    if (i > 0) std::cout << ",";

    std::cout << "\"" << xml_filename << "\":[";

    int j = 0;

    for (const auto& field : fields_in_xml_files_[i]) {
      if (j > 0) std::cout << ",";

      std::cout << "\"" << field << "\"";

      j++;
    }

    std::cout << "]";

    i++;
  }

  std::cout << "}";
}
