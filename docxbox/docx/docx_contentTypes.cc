// Copyright (c) 2020 gyselroth GmbH

#include <docxbox/docx/docx_contentTypes.h>

docx_contentTypes::docx_contentTypes(std::string path_content_types_xml) {
  path_content_types_xml_ = std::move(path_content_types_xml);
}

bool docx_contentTypes::ReplacePartname(const std::string &part_name) {
  // TODO(kay): implement

  // std::string override_partname_tag = RenderOverridePartnameTag(part_name)

  // if (!PartnameTagExists(override_partname_tag))
  // std::cout << "!"; return false

  // helper::String::Replace(xml, overide_partname_tag, replace_tag);

  return true;
}
