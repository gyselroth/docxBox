// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/docx/xml/docx_xml.h>

docx_xml::docx_xml(int argc, char **argv) {
  argc_ = argc;
  argv_ = argv;
}

bool docx_xml::IsXmlFileContainingText(const std::string &filename) {
  return
      !helper::String::EndsWith(filename, "/")
      && helper::String::Contains(filename, "word/")
      && helper::String::EndsWith(filename, ".xml")
      && !helper::String::EndsWith(filename, "fontTable.xml")
      && !helper::String::EndsWith(filename, "settings.xml")
      && !helper::String::EndsWith(filename, "style.xml")
      && !helper::String::EndsWith(filename, "stylesWithEffects.xml")
      && !helper::String::EndsWith(filename, "webSettings.xml");
}

bool docx_xml::SaveXml(bool compress) {
  if (compress) helper::Xml::CompressXml(xml_);

  return helper::File::WriteToNewFile(path_xml_file_, xml_);
}