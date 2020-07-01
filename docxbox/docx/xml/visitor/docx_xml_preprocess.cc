// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/docx/xml/visitor/docx_xml_preprocess.h>

docx_xml_preprocess::docx_xml_preprocess(
    int argc, const std::vector<std::string>& argv) : docx_xml(argc, argv) {
}

// Load and tidy XML of given file:
// - Read into properties:
//   - doc_: tinyxml2
//   - xml_: std::string of compressed XML markup
// - Remove dispensable (non textual/formatting) attributes and (empty) tags
// - Merge textual runs (<w:r><w:t></w:t></w:r>)
//   that repeat the same (or no) run-properties in direct succession
bool docx_xml_preprocess::LoadXmlTidy(const std::string& path_xml) {
  helper::File::GetFileContents(path_xml, &xml_);

  if (!helper::String::Contains(xml_, "w:document")
      || !helper::String::Contains(xml_, "w:body")) return false;

  path_xml_ = path_xml;

  helper::Xml::CompressXml(&xml_);

  RemoveDispensableTags();

  doc_.Parse(xml_.c_str());

  return doc_.ErrorID() == 0 && DefragmentXml();
}

bool docx_xml_preprocess::SaveDocToXml(bool decode) {
  SetXmlFromDoc();

  if (decode) DecodeXmlEntities();

  return helper::File::WriteToNewFile(path_xml_, xml_);
}

void docx_xml_preprocess::DecodeXmlEntities() {
  helper::String::ReplaceAll(&xml_, "_LT_", "<");
  helper::String::ReplaceAll(&xml_, "_GT_", ">");
}

void docx_xml_preprocess::RemoveDispensableTags() {
  if (helper::String::Contains(xml_, "<w:rFonts "))
    helper::String::Remove(&xml_, std::regex(R"(<w:rFonts w:hint="\w+"/>)"));

  helper::String::RemoveAll(&xml_, "<w:lang/>");

  if (helper::String::Contains(xml_, "<w:lang "))
    helper::String::Remove(
        // e.g. <w:lang w:val="en-US"/>
        &xml_, std::regex(R"(<w:lang w:val="[a-zA-Z|-]{2,5}"/>)"));

  helper::String::RemoveAll(&xml_, "<w:noProof/>");
  helper::String::RemoveAll(&xml_, "<w:noProof w:val=\"true\"/>");
  helper::String::RemoveAll(&xml_, "<w:noProof w:val=\"false\"/>");

  helper::String::RemoveAll(&xml_, "<w:proofErr/>");

  if (helper::String::Contains(xml_, "<w:proofErr "))
    helper::String::Remove(
        &xml_, std::regex(R"(<w:proofErr w:type="\w+"/>)"));

  // TODO(kay): re-insert into all w:t and w:instrText nodes before saving
  helper::String::ReplaceAll(&xml_, "xml:space=\"preserve\"", "");

  // Remove empty tags
  helper::String::Remove(&xml_, std::regex(R"(<w:([a-zA-Z]+)></w:([a-zA-Z]+)>)"));
  helper::String::RemoveAll(&xml_, "<w:rPr></w:rPr>");
  helper::String::RemoveAll(&xml_, "<w:rPr/>");
}

bool docx_xml_preprocess::DefragmentXml() {
  tinyxml2::XMLElement *body =
      doc_.FirstChildElement("w:document")->FirstChildElement("w:body");

  InitMarRunsToBeMerged();
  MarkRunsToBeMerged(body);

  if (!found_runs_to_be_merged_) return true;

  return MergeNodes();
}

void docx_xml_preprocess::InitMarRunsToBeMerged() {
  run_index_ = 0;
  is_within_run_properties_ = false;
  previous_run_properties_ = {};
  properties_of_runs_ = {};
  is_first_run_of_section = true;
}

void docx_xml_preprocess::MarkRunsToBeMerged(tinyxml2::XMLElement *node) {
  if (!node || node->NoChildren()) return;

  tinyxml2::XMLElement *sub_node = node->FirstChildElement();

  if (sub_node == nullptr) return;

  do {
    ++run_index_;
    const char *tag = sub_node->Value();

    if (tag) {
      bool is_run_properties = false;

      if (0 == strcmp(tag, "w:p")) {
        is_within_run_properties_ = false;

        // 1st run of new section has no prev. sibling to merge w/
        is_first_run_of_section = true;
      } else if (0 == strcmp(tag, "w:r")) {
        is_within_run_properties_ = false;
      } else if (0 == strcmp(tag, "w:rPr")) {
        is_run_properties = true;
        is_within_run_properties_ = true;
        previous_run_properties_.clear();
      } else if (0 == strcmp(tag, "w:t") && sub_node->FirstChild() != nullptr) {
        properties_of_runs_.push_back(previous_run_properties_);

        if (is_first_run_of_section) {
          is_first_run_of_section = false;
        } else if (AreLastTwoRunPropertiesIdentical()) {
          std::string text = sub_node->GetText();

          std::string kText = "_MeRgE_Me_" + text;
          sub_node->SetText(kText.c_str());

          found_runs_to_be_merged_ = true;
        }
      }

      if (is_within_run_properties_ && !is_run_properties) {
        // Collect run's properties (tags inside <w:rPr>...</w:rPr>)
        // for when the tag needs to be split into multiple
        previous_run_properties_.emplace_back(tag);
      }
    }

    MarkRunsToBeMerged(sub_node);
  } while ((sub_node = sub_node->NextSiblingElement()));
}

bool docx_xml_preprocess::AreLastTwoRunPropertiesIdentical() {
  auto amount = properties_of_runs_.size();

  if (amount <= 1) return false;

  std::sort(properties_of_runs_.at(amount - 1).begin(),
            properties_of_runs_.at(amount - 1).end());

  std::sort(properties_of_runs_.at(amount - 2).begin(),
            properties_of_runs_.at(amount - 2).end());

  return
      helper::String::Implode(properties_of_runs_.at(amount - 1), "-")
          == helper::String::Implode(properties_of_runs_.at(amount - 2), "-");
}

// Locate occurrences of given string within textual nodes
// where the string is not the sole content: Split (into up to 3) nodes than
// to contain the given string in a tag of its own.
// Used as preprocessor before: rmt, rpt
bool docx_xml_preprocess::DissectXmlNodesContaining(const std::string& str) {
  texts_to_be_split_.clear();

  tinyxml2::XMLElement *kWDocument = doc_.FirstChildElement("w:document");

  if (nullptr == kWDocument) {
    return docxbox::AppLog::NotifyError(
        "Dissect XML: Failed parsing " + RemoveTmpEndingFromDocxPath(path_xml_)
        + ". Invalid XML?");
  }

  tinyxml2::XMLElement *body = kWDocument->FirstChildElement("w:body");

  LocateNodesContaining(body, str);

  if (texts_to_be_split_.empty()) return false;

  return SplitNodes(str.c_str());
}

// Locate w:t nodes containing given needle,
// remember their parent run's revision section ID (w:rsidRPr)
void docx_xml_preprocess::LocateNodesContaining(tinyxml2::XMLElement *node,
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

        if (helper::String::Contains(text, str)) {
          texts_to_be_split_.push_back(sub_node);
          properties_of_runs_.push_back(previous_run_properties_);
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

bool docx_xml_preprocess::MergeNodes() {
  SetXmlFromDoc();

  helper::String::Remove(
      &xml_,
      std::regex(
          "(<\\/w:t>|<w:t\\/>)\\s*</w:r>\\s*<w:r>\\s*"
          "(<w:rPr)*(/)*(>)*\\s*"
          "(<[/a-z0-9: =\"]*>)*\\s*"
          "(</w:rPr>)*\\s*"
          "<w:t>_MeRgE_Me_",
          std::regex_constants::icase));

  helper::String::RemoveAll(&xml_, "_MeRgE_Me_");

  SetDocFromXml();

  return true;
}

bool docx_xml_preprocess::SplitNodes(const char *str) {
  int index = 0;

  for (tinyxml2::XMLElement *node : texts_to_be_split_) {
    std::string node_type = node->Value();    // e.g. "w:t"
    std::string node_text = node->GetText();

    if (helper::String::StartsWith(&node_text, str)) {
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

  texts_to_be_split_.clear();
  properties_of_runs_.clear();

  SetXmlFromDoc();

  return true;
}

std::string docx_xml_preprocess::RenderSplitAfter(
    const char *str, const std::string &node_text, int index) const {
  return std::string(str)
            + "_LT_/w:t_GT__LT_/w:r_GT_"
              "_LT_w:r_GT_"
              + RenderRunProperties(index)
              + "_LT_w:t_GT_"
              + node_text.substr(strlen(str));
}

std::string docx_xml_preprocess::RenderSplitBefore(
    const char *str, const std::string &node_text, int index) const {
  return node_text.substr(0, node_text.length() - strlen(str))
      + "_LT_/w:t_GT__LT_/w:r_GT_"
        "_LT_w:r_GT_"
        + RenderRunProperties(index)
        + "_LT_w:t_GT_"
      + str;
}

std::string docx_xml_preprocess::RenderSplitAround(
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

std::string docx_xml_preprocess::RenderRunProperties(int index) const {
  auto properties = properties_of_runs_.at(index);

  if (properties.empty()) return "";

  std::string markup = "_LT_w:rPr_GT_";

  for (std::string &tag_name : properties)
    markup += "_LT_" + tag_name + "/_GT_";

  return markup + "_LT_/w:rPr_GT_";
}
