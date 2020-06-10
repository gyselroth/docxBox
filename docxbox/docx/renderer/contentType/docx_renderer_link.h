// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_DOCX_RENDERER_CONTENTTYPE_DOCX_RENDERER_LINK_H_
#define DOCXBOX_DOCX_RENDERER_CONTENTTYPE_DOCX_RENDERER_LINK_H_

#include <docxbox/docx/component/numbering.h>
#include <docxbox/docx/renderer/docx_renderer.h>
#include <docxbox/helper/helper_string.h>

#include <string>
#include <utility>
#include <vector>

#include <vendor/json/single_include/nlohmann/json.hpp>

class docx_renderer_link: public docx_renderer {
 public:
  // Constructor
  docx_renderer_link(std::string path_extract, const std::string &json);

  void SetRelationshipId(const std::string &relationship_id);

  std::string Render(const std::string &relationship_id);
  std::string Render() override;

  std::string generic_root_tag_;

 private:
  std::string text_;
  std::string url_;

  std::string relationship_id_;

  bool InitFromJson() override;
};

#endif  // DOCXBOX_DOCX_RENDERER_CONTENTTYPE_DOCX_RENDERER_LINK_H_
