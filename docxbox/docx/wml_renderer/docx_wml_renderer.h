// Copyright (c) 2020 gyselroth GmbH

#ifndef DOCXBOX_DOCX_WML_RENDERER_DOCX_WML_RENDERER_H_
#define DOCXBOX_DOCX_WML_RENDERER_DOCX_WML_RENDERER_H_

#include <string>

static const char *const kWRunLhs = "<w:r>";
static const char *const kWRunRhs = "</w:r>";

static const char *const kWTableLhs = "<w:tbl>";
static const char *const kWTableRhs = "</w:tbl>";

class docx_wml_renderer {
 protected:
  std::string json_;

  bool is_valid_table_json_;

  std::string wml_;
};

#endif  // DOCXBOX_DOCX_WML_RENDERER_DOCX_WML_RENDERER_H_
