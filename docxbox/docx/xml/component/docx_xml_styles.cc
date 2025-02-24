// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/docx/xml/component/docx_xml_styles.h>
#include <docxbox/docx/component/numbering.h>
#include <docxbox/docx/component/rels.h>

#include <utility>

docx_xml_styles::docx_xml_styles(
    std::string path_extract,
    int argc,
    const std::vector<std::string> &argv): docx_xml(argc, argv) {
  path_xml_file_ = path_extract + "/word/styles.xml";
  path_extract_ = std::move(path_extract);

  helper::File::GetFileContents(path_xml_file_, &xml_);
}

// Get (insert if not exists) style ID
std::string docx_xml_styles::GetStyleId(
    styles::StyleType type, styles::StyleName name) {
  // TODO(kay): implement find/insert style

  doc_.Parse(xml_.c_str());

  if (doc_.ErrorID() != 0) {
    docxbox::AppLog::NotifyError("Failed parse word/styles.xml");

    return "";
  }

  tinyxml2::XMLElement *styles = doc_.FirstChildElement("w:styles");

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
//  if (!SaveXml(true))
//    docxbox::AppLog::NotifyError("Failed save word/styles.xml");

  return style_id;
}

std::string docx_xml_styles::GetStyleIdPrefixByType(
    styles::StyleType type) {
  switch (type) {
    case styles::StyleType::StyleType_Character:
      return "char";
    case styles::StyleType::StyleType_Paragraph:
      return "para";
    case styles::StyleType::StyleType_Table:
      return "Table";
  }
}
