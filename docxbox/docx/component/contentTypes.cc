// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/docx/component/contentTypes.h>
#include <docxbox/docx/xml/component/docx_xml_contentTypes.h>

const std::string contentTypes::MIME_CONTENT_TYPE_APP =
    "application/vnd.openxmlformats-officedocument.extended-properties+xml";

const std::string contentTypes::MIME_CONTENT_TYPE_BIN =
    "application/vnd.openxmlformats-officedocument.oleObject";

const std::string contentTypes::MIME_CONTENT_TYPE_CORE =
    "application/vnd.openxmlformats-package.core-properties+xml";

const std::string contentTypes::MIME_CONTENT_TYPE_DOCUMENT =
    "application/vnd.openxmlformats-officedocument.wordprocessingml."
    "document.main+xml";

const std::string contentTypes::MIME_CONTENT_TYPE_FONT_TABLE =
    "application/vnd.openxmlformats-officedocument.wordprocessingml."
    "fontTable+xml";

const std::string contentTypes::MIME_CONTENT_TYPE_NUMBERING =
    "application/vnd.openxmlformats-officedocument.wordprocessingml."
    "numbering+xml";

const std::string contentTypes::MIME_CONTENT_TYPE_SETTINGS =
    "application/vnd.openxmlformats-officedocument.wordprocessingml."
    "settings+xml";

const std::string contentTypes::MIME_CONTENT_TYPE_STYLES =
    "application/vnd.openxmlformats-officedocument.wordprocessingml.styles+xml";

const std::string contentTypes::MIME_CONTENT_TYPE_STYLES_WITH_EFFECTS =
    "application/vnd.ms-word.stylesWithEffects+xml";

const std::string contentTypes::MIME_CONTENT_TYPE_WEB_SETTINGS =
    "application/vnd.openxmlformats-officedocument.wordprocessingml."
    "webSettings+xml";

bool contentTypes::OverrideNumbering(std::string &path_extract_absolute) {
  auto component = new docx_xml_contentTypes(path_extract_absolute);

  auto result = component->OverrideNumbering();

  delete component;

  return result;
}
