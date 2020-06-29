// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/docx/xml/visitor/docx_xml_dissect.h>

docx_xml_dissect::docx_xml_dissect(
    int argc, const std::vector<std::string>& argv) : docx_xml(argc, argv) {
}

bool docx_xml_dissect::LoadXml(const std::string& path_xml) {
  helper::File::GetFileContents(path_xml, &xml_);

  if (!helper::String::Contains(xml_, "w:document")
      || !helper::String::Contains(xml_, "w:body")) return false;

  path_xml_ = path_xml;

  doc_.LoadFile(path_xml.c_str());

  return doc_.ErrorID() == 0;
}

bool docx_xml_dissect::SaveXml() {
  tinyxml2::XMLPrinter printer;
  doc_.Print(&printer);
  xml_ = printer.CStr();

  helper::String::ReplaceAll(&xml_, "&lt;/w:r&gt;", "</w:r>");
  helper::String::ReplaceAll(&xml_, "&lt;/w:t&gt;", "</w:t>");
  helper::String::ReplaceAll(&xml_, "&lt;w:t&gt;", "<w:t>");
  helper::String::ReplaceAll(&xml_, "&lt;w:r&gt;", "<w:r>");

  return helper::File::WriteToNewFile(path_xml_, xml_);
}

// Locate occurrences of given string within textual nodes
// where the string is not the sole content: Split (into up to 3) nodes than
// to contain the given string in a tag of its own.
// Used as preprocessor before: rmt, rpt
bool docx_xml_dissect::DissectXmlNodesContaining(const std::string& str) {
  nodes_to_be_split_.clear();

  tinyxml2::XMLElement *body =
      doc_.FirstChildElement("w:document")->FirstChildElement("w:body");

  LocateNodesContaining(body, str);

  if (nodes_to_be_split_.empty()) return false;

  return SplitNodes(str.c_str());
}

void docx_xml_dissect::LocateNodesContaining(tinyxml2::XMLElement *node,
                                             const std::string &str) {
  if (!node || node->NoChildren()) return;

  tinyxml2::XMLElement *sub_node = node->FirstChildElement();

  if (sub_node == nullptr) return;

  do {
    const char *tag = sub_node->Value();

    if (tag) {
      if (0 == strcmp(tag, "w:t")
          && sub_node->FirstChild() != nullptr) {
        std::string text = sub_node->GetText();

        if (text.empty() || text == str) continue;

        if (helper::String::Contains(text, str.c_str())) {
          nodes_to_be_split_.push_back(sub_node);
        }
      }
    }

    LocateNodesContaining(sub_node, str);
  } while ((sub_node = sub_node->NextSiblingElement()));
}

bool docx_xml_dissect::SplitNodes(const char *str) {
  for (tinyxml2::XMLElement *node : nodes_to_be_split_) {
    std::string node_type = node->Value();    // e.g. "w:t"
    std::string node_text = node->GetText();

    if (helper::String::StartsWith(node_text.c_str(), str)) {
      node->SetText(
          RenderSplitAfter(str, node_text).c_str());
    } else if (helper::String::EndsWith(node_text, str)) {
      node->SetText(
          RenderSplitBefore(str, node_text).c_str());
    } else {
      node->SetText(
          RenderSplitAround(str, node_text).c_str());
    }
  }

  nodes_to_be_split_.clear();

  return true;
}

std::string docx_xml_dissect::RenderSplitAfter(
    const char *str, const std::string &node_text) const {
  return std::string(str)
            + "</w:t></w:r>"
              "<w:r><w:t>"
              + node_text.substr(strlen(str));
}

std::string docx_xml_dissect::RenderSplitBefore(
    const char *str, const std::string &node_text) const {
  return node_text.substr(0, node_text.length() - strlen(str))
      + "</w:t></w:r>"
        "<w:r><w:t>"
      + str;
}

std::string docx_xml_dissect::RenderSplitAround(
    const char *str, const std::string &node_text) const {
  int offset_str = node_text.find(str);

  return node_text.substr(0, offset_str)
          + "</w:t></w:r>"
            "<w:r><w:t>"
          + str
          + "</w:t></w:r>"
            "<w:r><w:t>"
          + node_text.substr(offset_str + strlen(str));
}
