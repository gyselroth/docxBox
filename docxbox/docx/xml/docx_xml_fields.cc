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

bool docx_xml_fields::SetFieldText(
    const std::string& path_xml,
    const std::string &field_identifier,
    const std::string &text) {
  tinyxml2::XMLDocument doc;
  doc.LoadFile(path_xml.c_str());

  if (doc.ErrorID() != 0) return false;

  tinyxml2::XMLElement *body =
      doc.FirstChildElement("w:document")->FirstChildElement("w:body");

  is_inside_searched_field_ = false;

  SetFieldTextAndCollectFieldNodes(body, field_identifier, text);

  // TODO(kay): save only if changed
  if (tinyxml2::XML_SUCCESS != doc.SaveFile(path_xml.c_str(), true)) {
    std::cerr << "Error - Failed saving: " << path_xml << "\n";

    return false;
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

    bool found_field_end = false;

    if (value) {
      if (0 == strcmp(value, "w:fldChar")) {
        if (is_inside_searched_field_) {
          std::string type = sub_node->Attribute("w:fldCharType");

          if (type == "end") {
            found_field_end = true;
            is_inside_searched_field_ = false;
          }
        }
      } else if (0 == strcmp(value, "w:instrText")) {
        std::string iterated_field_identifier = sub_node->GetText();
        helper::String::Trim(iterated_field_identifier);

        if (helper::String::StartsWith(
            iterated_field_identifier.c_str(),
            field_identifier.c_str())) {
          is_inside_searched_field_ = true;

          // Remove rel. fldChar:separate if given
          auto prev_fldChar =
              sub_node->Parent()->PreviousSibling()->FirstChildElement("w:fldChar");

          if (prev_fldChar)
            prev_fldChar->Parent()->DeleteChild(prev_fldChar);

          // Remove rel. fldChar:begin if given
          prev_fldChar =
            sub_node->Parent()->PreviousSibling()->FirstChildElement("w:fldChar");

          if (prev_fldChar)
            prev_fldChar->Parent()->DeleteChild(prev_fldChar);

          // Remove instrText of field
          sub_node->Parent()->DeleteChild(sub_node);

          continue;
        }
      } else if (is_inside_searched_field_ && 0 == strcmp(value, "w:t")) {
        sub_node->SetText(field_value.c_str());
      }
    }

    if (found_field_end) {
      // Remove fldChar:end
      sub_node->Parent()->DeleteChild(sub_node);

      continue;
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
