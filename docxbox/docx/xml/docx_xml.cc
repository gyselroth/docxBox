// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/docx/xml/docx_xml.h>

docx_xml::docx_xml(int argc, const std::vector<std::string>& argv) {
  argc_ = argc;
  argv_ = argv;
}

bool docx_xml::IsXmlFileContainingText(const std::string &filename) {
  return !helper::String::EndsWith(filename, "/")
      && helper::String::EndsWith(filename, ".xml")
      && (helper::String::EndsWith(filename, "word/document.xml")
      || helper::String::Contains(filename, "word/header")
      || helper::String::Contains(filename, "word/endnotes.xml")
      || helper::String::Contains(filename, "word/footnotes.xml")
      || helper::String::Contains(filename, "word/footer"));
}

tinyxml2::XMLElement *docx_xml::GetBodyByComponentPath(
    tinyxml2::XMLDocument &doc, const std::string& path_xml) const {

  if (helper::String::Contains(path_xml, "word/endnotes.xml"))
    return doc.FirstChildElement("w:endnotes")->FirstChildElement("w:endnote");

  if (helper::String::Contains(path_xml, "word/footnotes.xml"))
    return
        doc.FirstChildElement("w:footnotes")->FirstChildElement("w:footnote");

  if (helper::String::Contains(path_xml, "word/header"))
    return doc.FirstChildElement("w:hdr");

  if (helper::String::Contains(path_xml, "word/footer"))
    return doc.FirstChildElement("w:ftr");

  // Default, e.g. word/document.xml
  return doc.FirstChildElement("w:document")->FirstChildElement("w:body");
}

bool docx_xml::SaveXml(bool compress) {
  if (compress) helper::Xml::CompressXml(xml_);

  return helper::File::WriteToNewFile(path_xml_file_, xml_);
}

void docx_xml::RemoveTmpEndingFromDocxPath(std::string &xml_filename) {
  int offset_docx_ending = xml_filename.find(".docx-");
  int offset_slash_after_docx = xml_filename.find('/', offset_docx_ending);

  xml_filename = xml_filename.substr(0, offset_docx_ending) + ".docx"
      + xml_filename.substr(offset_slash_after_docx);
}
