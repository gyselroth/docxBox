// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_DOCX_RENDERER_DOCX_RENDERER_LIST_H_
#define DOCXBOX_DOCX_RENDERER_DOCX_RENDERER_LIST_H_

#include <docxbox/docx/renderer/docx_renderer.h>
#include <docxbox/helper/helper_string.h>

#include <string>
#include <vector>

#include <vendor/json/single_include/nlohmann/json.hpp>

class docx_renderer_list: docx_renderer {
 public:
  // Constructor
  explicit docx_renderer_list(
      int argc, char **argv, const std::string &json);

  void SetIsOrdered(bool is_ordered = true);

  std::string Render();

 private:
  bool is_ordered_ = false;
  std::vector<std::string> items_;

  void InitSpecsFromJson();
};

#endif  // DOCXBOX_DOCX_RENDERER_DOCX_RENDERER_LIST_H_
