// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/docx/xml/visitor/docx_xml_remove.h>
#include "docx_xml_tidy.h"

docx_xml_remove::docx_xml_remove(
    int argc, const std::vector<std::string>& argv) : docx_xml(argc, argv) {}

bool docx_xml_remove::RemoveBetweenStringsInXml(const std::string& path_xml,
                                                const std::string &lhs,
                                                const std::string &rhs) {
  helper::File::GetFileContents(path_xml, &xml_);

  if (!helper::String::Contains(xml_, "w:document")
      || !helper::String::Contains(xml_, "w:body")) return true;

  SetDocFromXml();

  if (doc_.ErrorID() != 0) return false;

  tinyxml2::XMLElement *body =
      doc_.FirstChildElement("w:document")->FirstChildElement("w:body");

  index_parent_ = 0;
  found_lhs_ = false;
  found_rhs_ = false;

  LocateNodesForRemovalBetweenText(body, lhs.c_str(), rhs.c_str());

  if (found_lhs_ && found_rhs_
      && (!nodes_to_be_removed_.empty()
          || !parents_to_be_removed_.empty())) {
    amount_removed_ = 0;

    RemoveNodes(&nodes_to_be_removed_);
    RemoveNodes(&parents_to_be_removed_);

    SetXmlFromDoc();
    RemoveIndexesFromTags();
    docx_xml_tidy::RemoveDispensableTags(xml_);
    docx_xml_tidy::RestorePreserveSpace(xml_);
    SetDocFromXml();

    return tinyxml2::XML_SUCCESS != doc_.SaveFile(path_xml.c_str(), true)
           ? docxbox::AppLog::NotifyError("Failed saving: " + path_xml)
           : true;
  }

  return true;
}

void docx_xml_remove::LocateNodesForRemovalBetweenText(
    tinyxml2::XMLElement *node, const char *lhs, const char *rhs) {
  tinyxml2::XMLElement *sub_node = node->FirstChildElement();

  if (nullptr == sub_node) return;

  do {  // Iterate over all sibling nodes on current level
    const char *tag = sub_node->Value();

    if (0 == strcmp(tag, "w:t")) {
      if (!CollectTextNodesForRemoval(lhs, rhs, sub_node)) continue;
    } else {
      if (found_lhs_) RememberNodeAsParentToBeRemoved(sub_node);
    }

    if (found_rhs_) break;

    ++index_t_;

    // Iterate children of current node
    LocateNodesForRemovalBetweenText(sub_node, lhs, rhs);
  } while (!found_rhs_ && (sub_node = sub_node->NextSiblingElement()));
}

bool docx_xml_remove::CollectTextNodesForRemoval(const char *lhs,
                                                 const char *rhs,
                                                 tinyxml2::XMLElement *node) {
  if ((found_lhs_ && found_rhs_)) return false;

  auto tmp_text = node->GetText();
  std::string text = nullptr == tmp_text ? "" : std::string(tmp_text);

  if (!found_lhs_ && 0 == strcmp(lhs, text.c_str())) OnFoundLhs(node);

  if (found_lhs_ && !found_rhs_) {
    if (0 == strcmp(rhs, text.c_str())) {
      OnFoundRhs(node);
    } else if (found_lhs_) {
      // Collect runs after LHS until RHS is found
      AppendIndexToNodeTag(node, index_t_);
      nodes_to_be_removed_.push_back(node);
    }
  }

  return true;
}

void docx_xml_remove::OnFoundRhs(tinyxml2::XMLElement *node) {
  found_rhs_ = true;

  AppendIndexToNodeTag(node, index_t_);
  nodes_to_be_removed_.push_back(node);


  if (!parents_to_be_removed_.empty()) {
    // Exclude paragraph containing RHS from removal
    PopBackAncestorsFromRemoval(node);
  }
}

void docx_xml_remove::OnFoundLhs(tinyxml2::XMLElement *node) {
  found_lhs_ = true;

  AppendIndexToNodeTag(node, index_t_);
  PopBackPrecedingSiblingsFromRemoval(index_t_);

  nodes_to_be_removed_.push_back(node);
}

void docx_xml_remove::RememberNodeAsParentToBeRemoved(tinyxml2::XMLElement *node) {
  auto parent_tag = node->Parent()->Value();

  if (nullptr == parent_tag
      || 0 == strcmp("w:body", parent_tag)
      || 0 == strcmp("w:document", parent_tag)) return;

  AppendIndexToNodeTag(node, index_parent_);
  ++index_parent_;

  parents_to_be_removed_.push_back(node);
}

// Remove paragraphs of ancestry of node from removal stack
void docx_xml_remove::PopBackAncestorsFromRemoval(tinyxml2::XMLElement* node) {
  tinyxml2::XMLNode* parent = node->Parent();

  while (nullptr != parent) {
    const char *tag_parent = parent->Value();

    if (IsIndexedTag(tag_parent)) {
      for (auto iter = parents_to_be_removed_.begin();
           iter != parents_to_be_removed_.end(); ++iter) {
        const char* tag_para = (*iter)->Value();

        if (tag_parent == tag_para) {
          parents_to_be_removed_.erase(iter);
          break;
        }
      }
    }

    parent = parent->Parent();
  }
}

// Remove w:t sibling preceding w:t which contains LHS from removal stack
void docx_xml_remove::PopBackPrecedingSiblingsFromRemoval(
    unsigned int index_min) {
  for (auto iter = nodes_to_be_removed_.begin();
       iter != nodes_to_be_removed_.end(); ++iter) {
    auto tag = (*iter)->Value();

    if (nullptr == tag) return;

    auto index = ExtractIndexFromTagName(tag);

    if (index < index_min)
      parents_to_be_removed_.erase(iter);
  }
}

bool docx_xml_remove::RemoveNodes(std::vector<tinyxml2::XMLElement*> *nodes) {
  for (auto node : *nodes) {
    if (nullptr == node) continue;

    tinyxml2::XMLNode *kParent = node->Parent();

    if (kParent) {
      kParent->DeleteChild(node);

      ++amount_removed_;
    }
  }

  return amount_removed_ > 0;
}

int docx_xml_remove::GetAmountRemoved() {
  return amount_removed_;
}
