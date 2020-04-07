
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

std::string docx_xml::GetTextFromXmlFile(std::string path_xml, bool newlineAtSegments) {
  tinyxml2::XMLDocument doc;

  doc.LoadFile(path_xml.c_str());

  if(doc.ErrorID() != 0) return "";

  tinyxml2::XMLElement *body = doc.FirstChildElement("w:document")->FirstChildElement("w:body");
  GetChildNodesText(body, newlineAtSegments);

  return document_text;
}

void docx_xml::GetChildNodesText(tinyxml2::XMLElement *node, bool newlineAtSegments) {
  if (!node || node->NoChildren()) return;

  if (0 == strcmp(node->Value(), "w:p")) document_text += "\n";

  auto sub_node = node->FirstChildElement();

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
          ) document_text += " ";
        } else if (0 == strcmp(value, "w:t")) {
          const char *text = sub_node->GetText();

          if (text) {
            document_text += text;

            if (newlineAtSegments) document_text += "\n";
          }

          continue;
      }
    }

    GetChildNodesText(sub_node, newlineAtSegments);
  } while ((sub_node = sub_node->NextSiblingElement()));
}

void docx_xml::Output() {
  std::cout << document_text;
}