// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/docx/xml/visitor/docx_xml_replace.h>

docx_xml_replace::docx_xml_replace(
    int argc, const std::vector<std::string>& argv) : docx_xml(argc, argv) {
}

void docx_xml_replace::SetReplacementXmlFirstChildTag(
    const std::string &replacement_xml_first_child_tag) {
  replacement_xml_root_tag_ = replacement_xml_first_child_tag;
}

void docx_xml_replace::SetImageRelationshipId(
    const std::string &relationship_id) {
  image_relationship_id_ = relationship_id;
}

void docx_xml_replace::SetHyperlinkRelationshipId(
    const std::string &relationship_id) {
  hyperlink_relationship_id_ = relationship_id;
}

int docx_xml_replace::GetAmountReplaced() {
  return amount_replaced_;
}

// Replace given search string in given XML, update doc_ from it
// Replacement can be:
// 1. A regular string: will replace the given string
// 2. A string containing JSON: will be interpreted for rendering word markup,
//   which will than replace the <w:r> that contained the <w:t> node,
//   which contained the search-string
bool docx_xml_replace::ReplaceInXmlIntoDoc(std::string &xml,
                                           const std::string &search,
                                           const std::string &replacement) {
  is_replacement_xml_ = helper::Json::IsJson(replacement);

  if (is_replacement_xml_) {
    // Replacement will be done in steps:
    // 1. Inject pre-rendered XML into document
    // 2. Locate nodes containing child-nodes containing text to be replaced
    // 3. Replace the located nodes w/ deep-cloned duplicates of XML from 1.
    // 4. Remove injection from 1.
    runs_to_be_replaced_.clear();

    // Render and inject markup (initially) before body
    const std::string &kMarkup = RenderMarkupFromJson(replacement);

    if (kMarkup.empty())
      return docxbox::AppLog::NotifyError(
          "Failed render markup from given JSON");

    // Insert temporarily before body, will later be moved into correct place
    // TODO(kay): use resp. different root element instead of w:body,
    //            when not within document.xml
    helper::String::ReplaceAll(&xml, "<w:body>", kMarkup + "<w:body>");
  }

  doc_.Parse((xml).c_str());

  if (doc_.ErrorID() != 0) return false;

  amount_replaced_ = 0;

  if (is_replacement_xml_)
    replacement_xml_element_ =
        doc_.FirstChildElement()->FirstChildElement(
            replacement_xml_root_tag_.c_str());

  tinyxml2::XMLElement *body =
      doc_.FirstChildElement("w:document")->FirstChildElement("w:body");

  // Replace text, if replacement is markup: locate runs to be replaced
  ReplaceOrLocateStringInXml(body, search, replacement);

  if (is_replacement_xml_ && !runs_to_be_replaced_.empty())
    ReplaceRunsByXmlElement();

  return amount_replaced_ > 0;
}

// Replaces the searched string
// Pre: A single <w:t> node contains the full string (ensured via docx_xml_tidy)
void docx_xml_replace::ReplaceOrLocateStringInXml(
    tinyxml2::XMLElement *node,
    const std::string& search,
    const std::string& replacement) {
  if (!node || node->NoChildren()) return;

  tinyxml2::XMLElement *sub_node = node->FirstChildElement();

  if (sub_node == nullptr) return;

  do {
    if (!sub_node) continue;

    const char *tag = sub_node->Value();

    if (tag) {
      if (0 == strcmp(tag, "w:r")) {
          current_run_ = sub_node;
      } else if (0 == strcmp(tag, "w:t")
          && sub_node->FirstChild() != nullptr) {
        std::string text = sub_node->GetText();

        if (!text.empty()
            && helper::String::Contains(text, search)) {
          if (is_replacement_xml_) {
            runs_to_be_replaced_.push_back(current_run_);
          } else {
            amount_replaced_ +=
                helper::String::ReplaceAll(&text, search, replacement);

            sub_node->SetText(text.c_str());
          }
        }

        continue;
      }
    }

    ReplaceOrLocateStringInXml(sub_node, search, replacement);
  } while ((sub_node = sub_node->NextSiblingElement()));
}

// Step two of replacing text by markup
// (1: locate runs, containing nodes containing text to be replaced)
// 2: Replace located runs by markup
void docx_xml_replace::ReplaceRunsByXmlElement() {
  for (auto run : runs_to_be_replaced_) {
    if (nullptr == run) continue;

    tinyxml2::XMLNode *kParent = run->Parent();

    if (kParent) {
      auto replacement =
          replacement_xml_element_->DeepClone(
              replacement_xml_element_->GetDocument());

      kParent->InsertAfterChild(run, replacement);
      kParent->DeleteChild(run);
    }

    amount_replaced_++;
  }

  replacement_xml_element_->Parent()->DeleteChild(replacement_xml_element_);
}
