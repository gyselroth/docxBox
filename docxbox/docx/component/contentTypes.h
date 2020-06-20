// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_DOCX_COMPONENT_CONTENTTYPES_H_
#define DOCXBOX_DOCX_COMPONENT_CONTENTTYPES_H_

#include <docxbox/docx/component/partNames.h>

#include <string>
#include <utility>

class contentTypes {
 public:
  static const std::string MIME_CONTENT_TYPE_APP;
  static const std::string MIME_CONTENT_TYPE_BIN;
  static const std::string MIME_CONTENT_TYPE_CORE;
  static const std::string MIME_CONTENT_TYPE_DOCUMENT;
  static const std::string MIME_CONTENT_TYPE_FONT_TABLE;
  static const std::string MIME_CONTENT_TYPE_NUMBERING;
  static const std::string MIME_CONTENT_TYPE_SETTINGS;
  static const std::string MIME_CONTENT_TYPE_STYLES;
  static const std::string MIME_CONTENT_TYPE_STYLES_WITH_EFFECTS;
  static const std::string MIME_CONTENT_TYPE_WEB_SETTINGS;

  enum ContentType {
    ContentType_Hyperlink,
    ContentType_Image,
    ContentType_Numbering,
    ContentType_None
  };

  static bool OverrideNumbering(std::string &path_extract_absolute);
};

#endif  // DOCXBOX_DOCX_COMPONENT_CONTENTTYPES_H_
