// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/helper/helper_xml.h>

#include <vendor/tinyxml2/tinyxml2.h>

namespace helper {

bool Xml::IndentXmlFile(const std::string& path_xml) {
  tinyxml2::XMLDocument doc;

  doc.LoadFile(path_xml.c_str());

  if (doc.ErrorID() != 0) return false;

  return tinyxml2::XML_SUCCESS != doc.SaveFile(path_xml.c_str())
         ? docxbox::AppLog::NotifyError("Failed saving: " + path_xml)
         : true;
}

bool Xml::CompressXml(std::string &xml) {
  tinyxml2::XMLDocument doc;

  doc.Parse(xml.c_str());

  tinyxml2::XMLPrinter printer;
  doc.Print(&printer);
  xml = printer.CStr();

  return true;
}

}  // namespace helper
