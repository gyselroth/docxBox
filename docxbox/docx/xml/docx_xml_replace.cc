// Copyright (c) 2020 gyselroth GmbH

#include <docxbox/docx/xml/docx_xml_replace.h>

docx_xml_replace::docx_xml_replace(
    int argc,
    char **argv,
    bool is_replacement_xml) : docx_xml(argc, argv) {
  is_replacement_xml_ = is_replacement_xml;
}

bool docx_xml_replace::ReplaceStringInXml(
    const std::string& path_xml,
    const std::string& search,
    const std::string& replacement,
    bool replace_segmented
) {
  replace_segmented_ = replace_segmented;

  if (replace_segmented) {
    segments_look_behind_ = 0;
    previous_text_nodes_.clear();
  }

  if (is_replacement_xml_) {
    // Replacement will be done in steps:
    // 1. Locate nodes containing child-nodes containing text to be replaced
    // 2. Replace the located nodes
    runs_to_be_replaced_.clear();
  }

  tinyxml2::XMLDocument doc;

  std::string kPathXml = path_xml;
  std::string xml = helper::File::GetFileContents(kPathXml);

  if (!helper::String::Contains(xml, "w:document")
      || !helper::String::Contains(xml, "w:body")) return true;

  doc.LoadFile(path_xml.c_str());

  if (doc.ErrorID() != 0) return false;

  amount_replaced_ = 0;

  tinyxml2::XMLElement *body = doc
      .FirstChildElement("w:document")
      ->FirstChildElement("w:body");

  if (replace_segmented) {
    do {
      // Replace (regular and) segmented text
      document_text_ = "";

      ReplaceSegmentedStringInTextNodes(body, search, replacement);

      segments_look_behind_++;
    } while (helper::String::Contains(document_text_, search.c_str()));
  } else {
    // Replace only unsegmented text
    ReplaceStringInTextNodes(body, search, replacement);
  }

  if (is_replacement_xml_ && !runs_to_be_replaced_.empty())
    ReplaceRunsByMarkup(replacement);

  if (amount_replaced_ > 0
      && tinyxml2::XML_SUCCESS != doc.SaveFile(path_xml.c_str(), true)) {
    std::cerr << "Error - Failed saving: " << path_xml << "\n";

    return false;
  }

  return true;
}

// Replaces the searched string when contained within a single <w:t> node
void docx_xml_replace::ReplaceStringInTextNodes(
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
      //if (0 == strcmp(tag, "w:p")) {
          current_run_ = sub_node;
      } else if (0 == strcmp(tag, "w:t")
          && sub_node->FirstChild() != nullptr) {
        std::string text = sub_node->GetText();

        if (!text.empty()
            && helper::String::Contains(text, search.c_str())) {
          if (is_replacement_xml_) {
            runs_to_be_replaced_.push_back(current_run_);
          } else {
            amount_replaced_ +=
                helper::String::ReplaceAll(text, search, replacement);

            sub_node->SetText(text.c_str());
          }
        }

        continue;
      }
    }

    ReplaceStringInTextNodes(sub_node, search, replacement);
  } while ((sub_node = sub_node->NextSiblingElement()));
}

// Step two of replacing text by markup
// (1: locate runs, containing nodes containing text to be replaced)
// 2: Replace located runs by markup
void docx_xml_replace::ReplaceRunsByMarkup(const std::string &replacement) {
  tinyxml2::XMLDocument replacement_doc;
  replacement_doc.Parse(replacement.c_str());

  auto replacement_node = replacement_doc.FirstChildElement();

  for (auto run : runs_to_be_replaced_) {
    tinyxml2::XMLNode *kParent_node = run->Parent();

    kParent_node->InsertAfterChild(
        run,
        replacement_node);

    kParent_node->DeleteChild(run);

    amount_replaced_++;
  }
}

/*  Replaces the searched string when contained in a single <w:t> tag,
 *  and also when contained "segmented", that is:
 *  spread over multiple consecutive <w:t> nodes (due to formatting).
 *
 *  Steps for replacing segmented strings:
 *  1. Replace simple (look-behind level = 0),
 *     collecting document plaintext at the same time
 *  2. When plaintext of whole XML does not contain search-string anymore: done
 *  3. Increment look-behind level
 *  4. Iterate over text nodes, detect when concatenated text from
 *     previous nodes and current contains search-string
 *  5. When search-string found:
 *        - Replace matching text segment within 1st <w:t> node
 *          of current look-behind-sequence,
 *        - Remove matching text segments from other <w:t> nodes
 *          of current look-behind-sequence
 *  6. Repeat from step 2.
 */
void docx_xml_replace::ReplaceSegmentedStringInTextNodes(
    tinyxml2::XMLElement *node,
    const std::string& search,
    const std::string& replacement
) {
  if (!node || node->NoChildren()) return;

  if (0 == strcmp(node->Value(), "w:p")) document_text_ += "\n";

  tinyxml2::XMLElement *sub_node = node->FirstChildElement();

  if (sub_node == nullptr) return;

  do {
    if (!sub_node) continue;

    const char *value = sub_node->Value();

    if (value) {
      if (0 == strcmp(value, "w:t")
          && sub_node->FirstChild() != nullptr) {
        std::string text = sub_node->GetText();

        document_text_ += text;

        if (!text.empty()
            && helper::String::Contains(text, search.c_str())) {
          amount_replaced_ +=
              helper::String::ReplaceAll(text, search, replacement);

          sub_node->SetText(text.c_str());
        }

        continue;
      } else if (0 == strcmp(value, "w:fldChar")) {
        if (
            0 == strcmp(
                sub_node->Attribute("w:fldCharType"),
                "begin")) document_text_ += " ";
      } else if (0 == strcmp(value, "w:instrText")) {
        continue;
      }
    }

    ReplaceStringInTextNodes(sub_node, search, replacement);
  } while ((sub_node = sub_node->NextSiblingElement()));
}
