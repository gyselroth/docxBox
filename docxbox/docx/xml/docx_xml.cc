// Copyright (c) 2020 gyselroth GmbH

#include <docxbox/docx/xml/docx_xml.h>

docx_xml::docx_xml(int argc, char **argv) {
  argc_ = argc;
  argv_ = argv;
}

bool docx_xml::IsXmlFileContainingText(const std::string &filename) {
  return
      helper::String::Contains(filename, "word/")
      && !helper::String::EndsWith(filename, "fontTable.xml")
      && !helper::String::EndsWith(filename, "settings.xml")
      && !helper::String::EndsWith(filename, "style.xml")
      && !helper::String::EndsWith(filename, "webSettings.xml");
}

bool docx_xml::RemoveNodes(std::vector<tinyxml2::XMLElement*> nodes) {
  int amount_removed = 0;

  for (auto node : nodes) {
    if (nullptr == node) continue;

    tinyxml2::XMLNode *kParent = node->Parent();

    if (kParent) {
      kParent->DeleteChild(node);

      ++amount_removed;
    }
  }

  return amount_removed > 0;
}
