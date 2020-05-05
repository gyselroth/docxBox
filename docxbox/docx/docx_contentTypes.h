// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_DOCX_DOCX_CONTENTTYPES_H_
#define DOCXBOX_DOCX_DOCX_CONTENTTYPES_H_

#include <string>
#include <utility>

class docx_contentTypes {
 public:
  explicit docx_contentTypes(std::string path_content_types_xml);

  bool ReplacePartname(const std::string &part_name);

 private:
  std::string path_extract_;
  std::string path_content_types_xml_;
};

#endif  // DOCXBOX_DOCX_DOCX_CONTENTTYPES_H_
