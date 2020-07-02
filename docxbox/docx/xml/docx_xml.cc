// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/docx/xml/docx_xml.h>

#include <utility>

docx_xml::docx_xml(int argc, std::vector<std::string> argv)
    : argc_(argc), argv_(std::move(argv)) {
}

std::string docx_xml::GetXml() {
  return xml_;
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
    tinyxml2::XMLDocument *doc, const std::string& path_xml) const {

  if (helper::String::Contains(path_xml, "word/endnotes.xml"))
    return (*doc).FirstChildElement("w:endnotes")
        ->FirstChildElement("w:endnote");

  if (helper::String::Contains(path_xml, "word/footnotes.xml"))
    return (*doc).FirstChildElement("w:footnotes")
        ->FirstChildElement("w:footnote");

  if (helper::String::Contains(path_xml, "word/header"))
    return (*doc).FirstChildElement("w:hdr");

  if (helper::String::Contains(path_xml, "word/footer"))
    return (*doc).FirstChildElement("w:ftr");

  // Default, e.g. word/document.xml
  return (*doc).FirstChildElement("w:document")->FirstChildElement("w:body");
}

void docx_xml::SetXmlFromDoc() {
  tinyxml2::XMLPrinter printer;
  doc_.Print(&printer);
  xml_ = printer.CStr();
}

void docx_xml::SetDocFromXml() {
  doc_.Parse(xml_.c_str());
}

bool docx_xml::SaveXml(bool compress) {
  if (compress) helper::Xml::CompressXml(&xml_);

  return helper::File::WriteToNewFile(path_xml_file_, xml_);
}

std::string docx_xml::RemoveTmpEndingFromDocxPath(
    const std::string &xml_filename) {
  int offset_docx_ending = xml_filename.find(".docx-");
  int offset_slash_after_docx = xml_filename.find('/', offset_docx_ending);

  return xml_filename.substr(0, offset_docx_ending) + ".docx"
      + xml_filename.substr(offset_slash_after_docx);
}

// Tag is e.g. "w:p_<INDEX>"?
bool docx_xml::IsIndexedTag(const char *tag) {
  return nullptr != tag
      && strlen(tag) >= 3 && tag[2] == 'p' && tag[3] == '_';
}

void docx_xml::AppendIndexToNodeTag(tinyxml2::XMLElement *node, int index) {
  std::string tag = node->Value();
  std::string p_tag_marked = tag + std::to_string(index);

  node->SetValue(p_tag_marked.c_str());
}

void docx_xml::RemoveIndexesFromTags() {
  SetXmlFromDoc();

  xml_ = std::regex_replace(xml_, std::regex("(<w:[a-z])+_\\d+"), "$1");
  xml_ = std::regex_replace(xml_, std::regex("(</w:[a-z])+_\\d+"), "$1");

  SetDocFromXml();
}

int docx_xml::ExtractIndexFromTagName(const char* tag) {
  auto parts = helper::String::Explode(std::string(tag), '_');

  if (parts.size() <= 1) return -1;

  return std::stoi(parts[1]);
}
