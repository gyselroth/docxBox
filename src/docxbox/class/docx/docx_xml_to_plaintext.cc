
#include "docx_xml_to_plaintext.h"

docx_xml_to_plaintext::docx_xml_to_plaintext(int argc, char **argv) : docx_xml(argc, argv) {

}

std::string docx_xml_to_plaintext::GetTextFromXmlFile(const std::string& path_xml, bool newline_at_segments) {
  tinyxml2::XMLDocument doc;

  doc.LoadFile(path_xml.c_str());

  if(doc.ErrorID() != 0) return "";

  tinyxml2::XMLElement *body = doc.FirstChildElement("w:document")->FirstChildElement("w:body");
  GetChildNodesText(body, newline_at_segments);

  return document_text_;
}

void docx_xml_to_plaintext::GetChildNodesText(tinyxml2::XMLElement *node, bool newline_at_segments) {
  if (!node || node->NoChildren()) return;

  if (0 == strcmp(node->Value(), "w:p")) document_text_ += "\n";

  tinyxml2::XMLElement *sub_node = node->FirstChildElement();

  if (sub_node == nullptr) return;

  do {
    if (!sub_node) continue;

    const char *value = sub_node->Value();

    if (value) {
      if (0 == strcmp(value, "w:instrText")) {
        continue;
      } else if (0 == strcmp(value, "w:fldChar")) {
        if (
            0 == strcmp(
                sub_node->Attribute("w:fldCharType"),
                "begin"
            )
            ) document_text_ += " ";
      } else if (0 == strcmp(value, "w:t")) {
        const char *text = sub_node->GetText();

        if (text) {
          document_text_ += text;

          if (newline_at_segments) document_text_ += "\n";
        }

        continue;
      }
    }

    GetChildNodesText(sub_node, newline_at_segments);
  } while ((sub_node = sub_node->NextSiblingElement()));
}

void docx_xml_to_plaintext::Output() {
  std::cout << document_text_;
}
