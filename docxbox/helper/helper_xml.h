// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_HELPER_HELPER_XML_H_
#define DOCXBOX_HELPER_HELPER_XML_H_

#include <docxbox/app/app_status.h>

#include <string>

namespace helper {
namespace Xml {

extern bool IndentXmlFile(const std::string& path_xml);

extern bool CompressXml(std::string &xml);

}  // namespace Xml
}  // namespace helper

#endif  // DOCXBOX_HELPER_HELPER_XML_H_
