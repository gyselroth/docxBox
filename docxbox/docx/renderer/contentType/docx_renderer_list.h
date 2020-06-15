// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_DOCX_RENDERER_CONTENTTYPE_DOCX_RENDERER_LIST_H_
#define DOCXBOX_DOCX_RENDERER_CONTENTTYPE_DOCX_RENDERER_LIST_H_

#include <docxbox/app/app_log.h>
#include <docxbox/docx/renderer/docx_renderer.h>
#include <docxbox/docx/component/numbering.h>
#include <docxbox/helper/helper_string.h>

#include <string>
#include <utility>
#include <vector>

#include <vendor/json/single_include/nlohmann/json.hpp>

class docx_renderer_list: public docx_renderer {
 public:
  // Constructor
  explicit docx_renderer_list(
      std::string path_extract, const std::string &json);

  void SetIsOrdered(bool is_ordered = true);

  std::string Render(bool is_ordered);
  std::string Render() override;

 private:
  bool is_ordered_ = false;
  std::vector<std::string> items_;

  bool InitFromJson() override;

  // Ensure word/numbering.xml exists
  void AddNumberingXml() const;

  // Ensure relation to word/numbering.xml in word/_rels/document.xml.rels
  void AddNumberingRels() const;
};

#endif  // DOCXBOX_DOCX_RENDERER_CONTENTTYPE_DOCX_RENDERER_LIST_H_
