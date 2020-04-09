
#include "docx_xml_fields.h"

docx_xml_fields::docx_xml_fields(int argc, char **argv) : docx_xml(argc, argv) {

}

void docx_xml_fields::CollectMergeFields(std::string path_xml, bool as_json) {
  tinyxml2::XMLDocument doc;

  doc.LoadFile(path_xml.c_str());

  if(doc.ErrorID() != 0) return;

  tinyxml2::XMLElement *body = doc.FirstChildElement("w:document")->FirstChildElement("w:body");
  CollectFieldsFromNodes(body);
}

void docx_xml_fields::CollectFieldsFromNodes(tinyxml2::XMLElement *node) {
  if (!node || node->NoChildren()) return;

  //if (0 == strcmp(node->Value(), "w:p")) document_text_ += "\n";

  tinyxml2::XMLElement *sub_node = node->FirstChildElement();

  if (sub_node == nullptr) return;

  do {
    if (!sub_node) continue;

    const char *value = sub_node->Value();

    if (value) {
      if (0 == strcmp(value, "w:instrText")) {
        // TODO collect

        continue;
      } else if (0 == strcmp(value, "w:fldChar")) {
        /*if (
            0 == strcmp(
                sub_node->Attribute("w:fldCharType"),
                "begin"
            )
            ) document_text_ += " ";*/
      }
    }

    CollectFieldsFromNodes(sub_node);
  } while ((sub_node = sub_node->NextSiblingElement()));
}

void docx_xml_fields::Output() {
  std::cout << fields_;
}
