// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/docx/xml/component/docx_xml_contentTypes.h>
#include <docxbox/docx/component/contentTypes.h>

#include <utility>

docx_xml_contentTypes::docx_xml_contentTypes(
    std::string path_extract, int argc, char **argv) : docx_xml(
    argc,
    argv) {
  path_xml_file_ = path_extract + "/[Content_Types].xml";
  path_extract_ = std::move(path_extract);

  xml_ = helper::File::GetFileContents(path_xml_file_);
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

