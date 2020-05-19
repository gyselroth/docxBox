// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_DOCX_RENDERER_DOCX_RENDERER_LIST_H_
#define DOCXBOX_DOCX_RENDERER_DOCX_RENDERER_LIST_H_

#include <docxbox/app/app_error.h>
#include <docxbox/docx/renderer/docx_renderer.h>
#include <docxbox/docx/docx_numbering.h>
#include <docxbox/helper/helper_string.h>

#include <string>
#include <vector>

#include <vendor/json/single_include/nlohmann/json.hpp>

class docx_renderer_list: docx_renderer {
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
};

#endif  // DOCXBOX_DOCX_RENDERER_DOCX_RENDERER_LIST_H_
