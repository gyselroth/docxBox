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

  DecodeXmlEntities();

  return helper::File::WriteToNewFile(path_xml_, xml_);
}

void docx_xml_dissect::DecodeXmlEntities() {
  helper::String::ReplaceAll(&xml_, "_LT_", "<");
  helper::String::ReplaceAll(&xml_, "_GT_", ">");
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

// Locate w:t nodes containing given needle,
// remember their parent run's revision section ID (w:rsidRPr)
void docx_xml_dissect::LocateNodesContaining(tinyxml2::XMLElement *node,
                                             const std::string &str) {
  if (!node || node->NoChildren()) return;

  tinyxml2::XMLElement *sub_node = node->FirstChildElement();

  if (sub_node == nullptr) return;

  do {
    const char *tag = sub_node->Value();

    if (tag) {
      bool is_run_properties = false;

      if (0 == strcmp(tag, "w:r")) {
          is_within_run_properties_ = false;
      } else if (0 == strcmp(tag, "w:rPr")) {
        is_run_properties = true;
        is_within_run_properties_ = true;

        previous_run_properties_.clear();
      } else if (0 == strcmp(tag, "w:t")
          && sub_node->FirstChild() != nullptr) {
        std::string text = sub_node->GetText();

        if (text.empty() || text == str) continue;

        if (helper::String::Contains(text, str.c_str())) {
          nodes_to_be_split_.push_back(sub_node);
          nodes_run_properties_.push_back(previous_run_properties_);
        }
      }

      if (is_within_run_properties_ && !is_run_properties) {
        // Collect run's properties (tags inside <w:rPr>...</w:rPr>)
        // for when the tag needs to be split into multiple
        previous_run_properties_.emplace_back(tag);
      }
    }

    LocateNodesContaining(sub_node, str);
  } while ((sub_node = sub_node->NextSiblingElement()));
}

bool docx_xml_dissect::SplitNodes(const char *str) {
  int index = 0;

  for (tinyxml2::XMLElement *node : nodes_to_be_split_) {
    std::string node_type = node->Value();    // e.g. "w:t"
    std::string node_text = node->GetText();

    if (helper::String::StartsWith(node_text.c_str(), str)) {
      node->SetText(
          RenderSplitAfter(str, node_text, index).c_str());
    } else if (helper::String::EndsWith(node_text, str)) {
      node->SetText(
          RenderSplitBefore(str, node_text, index).c_str());
    } else {
      node->SetText(
          RenderSplitAround(str, node_text, index).c_str());
    }

    ++index;
  }

  nodes_to_be_split_.clear();
  nodes_run_properties_.clear();

  return true;
}

std::string docx_xml_dissect::RenderSplitAfter(
    const char *str, const std::string &node_text, int index) const {
  return std::string(str)
            + "_LT_/w:t_GT__LT_/w:r_GT_"
              "_LT_w:r_GT_"
              + RenderRunProperties(index)
              + "_LT_w:t_GT_"
              + node_text.substr(strlen(str));
}

std::string docx_xml_dissect::RenderSplitBefore(
    const char *str, const std::string &node_text, int index) const {
  return node_text.substr(0, node_text.length() - strlen(str))
      + "_LT_/w:t_GT__LT_/w:r_GT_"
        "_LT_w:r_GT_"
        + RenderRunProperties(index)
        + "_LT_w:t_GT_"
      + str;
}

std::string docx_xml_dissect::RenderSplitAround(
    const char *str, const std::string &node_text, int index) const {
  int offset_str = node_text.find(str);

  std::string run_properties = RenderRunProperties(index);

  return node_text.substr(0, offset_str)
          + "_LT_/w:t_GT__LT_/w:r_GT_"
            "_LT_w:r_GT_"
            + run_properties
            + "_LT_w:t_GT_"
          + str
          + "_LT_/w:t_GT__LT_/w:r_GT_"
            "_LT_w:r_GT_"
          + run_properties
          + "_LT_w:t_GT_"
          + node_text.substr(offset_str + strlen(str));
}

std::string docx_xml_dissect::RenderRunProperties(int index) const {
  auto properties = nodes_run_properties_.at(index);

  if (properties.empty()) return "";

  std::string markup = "_LT_w:rPr_GT_";

  for (std::string &tag_name : properties)
    markup += "_LT_" + tag_name + "/_GT_";

  return markup + "_LT_/w:rPr_GT_";
}
