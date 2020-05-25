// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/docx/xml/component/docx_xml_contentTypes.h>
#include <docxbox/docx/component/contentTypes.h>

#include <utility>
#include <docxbox/docx/xml/visitor/docx_xml_indent.h>

docx_xml_contentTypes::docx_xml_contentTypes(
    std::string path_extract, int argc, char **argv) : docx_xml(
    argc,
    argv) {
  path_contentTypes_xml = path_extract + "/[Content_Types].xml";
  path_extract_ = std::move(path_extract);

  xml_ = helper::File::GetFileContents(path_contentTypes_xml);
}

bool docx_xml_contentTypes::OverrideNumbering() {
  if (helper::String::Contains(
      xml_,
      "<Override PartName=\"/word/numbering.xml\""
      )) return true;

  helper::String::Replace(
      xml_,
      "</Types>",

      (std::string(R"(<Override PartName="/word/numbering.xml" ContentType=")")
      + kMimeContentTypeNumbering + "\"/></Types>").c_str());

  return SaveXml(false);
}

// TODO(kay): generalize xml-filename, move into parent class (docx_xml)
bool docx_xml_contentTypes::SaveXml(bool compress) {
  if (compress) docx_xml_indent::CompressXml(xml_);

  return helper::File::WriteToNewFile(path_contentTypes_xml, xml_);
}
