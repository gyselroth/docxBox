
#include "docx_xml_replace.h"

docx_xml_replace::docx_xml_replace(int argc, char **argv) : docx_xml(argc, argv) {

}

bool docx_xml_replace::ReplaceStringInXml(
    const std::string& path_xml,
    const std::string& search,
    const std::string& replacement,
    bool replace_segmented
) {
  this->replace_segmented = replace_segmented;

  if (replace_segmented) {
    segments_look_behind = 0;
    previous_text_nodes.clear();
  }

  tinyxml2::XMLDocument doc;

  std::string kPathXml = path_xml;
  std::string xml = helper::File::GetFileContents(kPathXml);
  if (!helper::String::Contains(xml, "w:document")
      || !helper::String::Contains(xml, "w:body")) return true;

  doc.LoadFile(path_xml.c_str());

  if(doc.ErrorID() != 0) return false;

  amount_replaced = 0;

  tinyxml2::XMLElement *body = doc.FirstChildElement("w:document")->FirstChildElement("w:body");

  if (replace_segmented) {
    do {
      document_text = "";

      ReplaceStringInChildNodesText(body, search, replacement);

      segments_look_behind++;
    } while (helper::String::Contains(document_text, search.c_str()));
  } else ReplaceStringInChildNodesText(body, search, replacement);

  if (
      amount_replaced > 0
          && tinyxml2::XML_SUCCESS != doc.SaveFile(path_xml.c_str(), true)
      ) {
    std::cout << "Error - Failed saving: " << path_xml << "\n";

    return false;
  }

  return true;
}

/*
 * Replacement has 2 modes:
 *
 * 1. Simple: Replaces the searched string only when contained within a single <w:t> node
 * 2. Segmented:
 *    Replaces the searched string as in 1,
 *    and also when contained "segmented", that is spread over multiple consecutive <w:t> nodes (due to formatting).
 *    These are the steps for replacing segmented strings:
 *    2.1 Replace simple (look-behind level = 0), collecting document plaintext at the same time
 *    2.2 When plaintext of whole XML does not contain search-string anymore: done
 *    2.3 Increment look-behind level
 *    2.4 Iterate over text nodes, detect when concatenated text from previous nodes and current contains search-string
 *    2.5 When search-string found:
 *          Replace matching text segment within 1st <w:t> node of current look-behind-sequence,
 *          Remove matching text segments from other <w:t> nodes of current look-behind-sequence
 *    2.6 Repeat with step 2.2
 */
void docx_xml_replace::ReplaceStringInChildNodesText(
    tinyxml2::XMLElement *node,
    const std::string& search,
    const std::string& replacement
) {
  if (!node || node->NoChildren()) return;

  tinyxml2::XMLElement *sub_node = node->FirstChildElement();

  if (sub_node == nullptr) return;

  do {
    if (!sub_node) continue;

    const char *value = sub_node->Value();

    if (value) {
      if (
          0 == strcmp(value, "w:t")
              && sub_node->FirstChild() != nullptr
          ) {
        std::string text = sub_node->GetText();

        if (!text.empty()
            && helper::String::Contains(text, search.c_str())
            ) {
          amount_replaced += helper::String::ReplaceAll(text, search, replacement);
          sub_node->SetText(text.c_str());
        }

        // @todo implement replacement of strings scattered over multiple xml tags (replace within first tag, set others empty)

        continue;
      } else if (0 == strcmp(value, "w:instrText")) {
        continue;
      }
    }

    ReplaceStringInChildNodesText(sub_node, search, replacement);
  } while ((sub_node = sub_node->NextSiblingElement()));
}