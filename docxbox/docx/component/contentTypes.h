// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_DOCX_COMPONENT_CONTENTTYPES_H_
#define DOCXBOX_DOCX_COMPONENT_CONTENTTYPES_H_

#include <docxbox/docx/component/partNames.h>

#include <string>
#include <utility>

static const char *const kMimeContentTypeApp =
    "application/vnd.openxmlformats-officedocument.extended-properties+xml";

static const char *const kMimeContentTypeBin =
    "\"application/vnd.openxmlformats-officedocument.oleObject";

static const char *const kMimeContentTypeCore =
    "application/vnd.openxmlformats-package.core-properties+xml";

static const char *const kMimeContentTypeDocument =
    "application/vnd.openxmlformats-officedocument.wordprocessingml."
    "document.main+xml";

static const char *const kMimeContentTypeFontTable =
    "application/vnd.openxmlformats-officedocument.wordprocessingml."
    "fontTable+xml";

static const char *const kMimeContentTypeNumbering =
    "application/vnd.openxmlformats-officedocument.wordprocessingml."
    "numbering+xml";

static const char *const kMimeContentTypeSettings =
    "application/vnd.openxmlformats-officedocument.wordprocessingml."
    "settings+xml";

static const char *const kMimeContentTypeStyles =
    "application/vnd.openxmlformats-officedocument.wordprocessingml.styles+xml";

static const char *const kMimeContentTypeStylesWithEffects =
    "application/vnd.ms-word.stylesWithEffects+xml";

static const char *const kMimeContentTypeWebSettings =
    "application/vnd.openxmlformats-officedocument.wordprocessingml."
    "webSettings+xml";

class contentTypes {
 public:
  enum ContentType {
    ContentType_Hyperlink,
    ContentType_Image,
    ContentType_Numbering,
    ContentType_None
  };

  static bool OverrideNumbering(std::string &path_extract_absolute);
};

#endif  // DOCXBOX_DOCX_COMPONENT_CONTENTTYPES_H_
