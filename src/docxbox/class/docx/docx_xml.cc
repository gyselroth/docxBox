
#include "docx_xml.h"

docx_xml::docx_xml(int argc, char **argv) {
  this->argc = argc;
  this->argv = argv;
}

bool docx_xml::IsXmlFileContainingText(std::string filename) {
  return
      helper::String::Contains(filename, "word/")
      && !helper::String::EndsWith(filename, "fontTable.xml")
      && !helper::String::EndsWith(filename, "settings.xml")
      && !helper::String::EndsWith(filename, "style.xml")
      && !helper::String::EndsWith(filename, "webSettings.xml");
}

bool docx_xml::ReplaceStringInXml(const std::string& path_xml, std::string search, std::string replacement) {
  tinyxml2::XMLDocument doc;

  std::string kPathXml = path_xml;
  std::string xml = helper::File::GetFileContents(kPathXml);
  if (!helper::String::Contains(xml, "w:document")
      || !helper::String::Contains(xml, "w:body")) return true;

  doc.LoadFile(path_xml.c_str());

  if(doc.ErrorID() != 0) return false;

  amount_replaced = 0;

  tinyxml2::XMLElement *body = doc.FirstChildElement("w:document")->FirstChildElement("w:body");

  ReplaceStringInChildNodesText(body, search, replacement);

  if (
      amount_replaced > 0
      && tinyxml2::XML_SUCCESS != doc.SaveFile(path_xml.c_str(), true)
  ) {
    std::cout << "Error - Failed saving: " << path_xml << "\n";

    return false;
  }

  return true;
}

void docx_xml::ReplaceStringInChildNodesText(
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