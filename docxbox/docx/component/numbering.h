// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_DOCX_COMPONENT_NUMBERING_H_
#define DOCXBOX_DOCX_COMPONENT_NUMBERING_H_

#include <docxbox/docx/component/rels.h>
#include <docxbox/helper/helper_file.h>
#include <docxbox/helper/helper_numeric.h>
#include <docxbox/helper/helper_string.h>

#include <cstring>
#include <iostream>
#include <utility>
#include <string>
#include <vector>

class numbering {
 public:
  explicit numbering(std::string path_extract);

  bool AddNumberingXml(bool unordered = true);

 private:
  std::string path_extract_;

  static std::string RenderXml(bool unordered = true);
  static std::string RenderXmlNamespaceTag();
};

#endif  // DOCXBOX_DOCX_COMPONENT_NUMBERING_H_
