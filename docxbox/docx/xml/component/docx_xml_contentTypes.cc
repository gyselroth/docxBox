// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/docx/xml/component/docx_xml_contentTypes.h>
#include <docxbox/docx/component/contentTypes.h>

#include <utility>

docx_xml_contentTypes::docx_xml_contentTypes(
    std::string path_extract,
    int argc,
    const std::vector<std::string>& argv)
    : docx_xml(argc, argv), path_extract_(std::move(path_extract)) {
  path_xml_file_ = path_extract + "/[Content_Types].xml";

  helper::File::GetFileContents(path_xml_file_, &xml_);
}

bool docx_xml_contentTypes::AddOverrideNumberingReference() {
  if (helper::String::Contains(
      xml_,
      "<Override PartName=\"/word/numbering.xml\""
      )) return true;

  helper::String::Replace(
      &xml_,
      "</Types>",

      (std::string(R"(<Override PartName="/word/numbering.xml" ContentType=")")
      + contentTypes::MIME_CONTENT_TYPE_NUMBERING + "\"/></Types>").c_str());

  return SaveXml(false);
}

