// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/docx/component/contentTypes.h>
#include <docxbox/docx/xml/component/docx_xml_contentTypes.h>

bool contentTypes::OverrideNumbering(std::string &path_extract_absolute) {
  auto component = new docx_xml_contentTypes(path_extract_absolute);

  auto result = component->OverrideNumbering();

  delete component;

  return result;
}
