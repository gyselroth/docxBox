// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/docx/xml/docx_xml_styles.h>
#include <docxbox/docx/docx_numbering.h>
#include <docxbox/docx/docx_rels.h>

#include <utility>

docx_xml_styles::docx_xml_styles(
    std::string path_extract, int argc, char **argv) : docx_xml(
    argc,
    argv) {
  path_styles_xml = path_extract + "/word/styles.xml";
  path_extract_ = std::move(path_extract);
  xml_ = helper::File::GetFileContents(path_styles_xml);
}

// Get (insert if not exists) style ID
std::string docx_xml_styles::GetStyleId(
    docx_styles::StyleType type, docx_styles::StyleName name) {
  // TODO(kay): implement find/insert style

  tinyxml2::XMLDocument doc;

  doc.Parse(xml_.c_str());

  if (doc.ErrorID() != 0) throw "Failed parse word/styles.xml";

  tinyxml2::XMLElement *styles = doc.FirstChildElement("w:styles");

  auto style = styles->FirstChildElement();

  const char* target_key = "Target";

  bool target_exists = false;

  std::string style_id;
//
//  do {
//    auto attr_target = style->FindAttribute(target_key);
//    if (!attr_target) continue;
//
//    std::string target_value = attr_target->Value();
//
//    GetStyleId(style, style_id);
//
//    if (target_value == target) {
//      target_exists = true;
//      break;
//    }
//  } while ((relationship = relationship->NextSiblingElement()));
//
//  if (target_exists) return style_id;
//
//  if (style_id.length() < 4) return "";
//
//  auto last_id = std::stoi(style_id.substr(3));
//
//  style_id = GetStyleIdPrefixByType(type) + + std::to_string(last_id + 1);
//
//  const std::basic_string<char,
//                          std::char_traits<char>,
//                          std::allocator<char>>
//      &kRelationshipTag = docx_renderer_rels::RenderRelationship(
//      target,
//      relation_type,
//      style_id) + "</w:styles>";
//
//  helper::String::Replace(xml_, "</w:styles>", kRelationshipTag.c_str());
//
//  if (!SaveXml(true)) throw "Failed save word/styles.xml\n";

  return style_id;
}

bool docx_xml_styles::SaveXml(bool compress) {
  if (compress) docx_xml_indent::CompressXml(xml_);

  helper::File::Remove(path_styles_xml.c_str());

  return helper::File::WriteToNewFile(path_styles_xml, xml_);
}

std::string docx_xml_styles::GetStyleIdPrefixByType(
    docx_styles::StyleType type) {
  switch (type) {
    case docx_styles::StyleType::StyleType_Character:
      return "char";
    case docx_styles::StyleType::StyleType_Paragraph:
      return "para";
    case docx_styles::StyleType::StyleType_Table:
      return "Table";
  }
}
