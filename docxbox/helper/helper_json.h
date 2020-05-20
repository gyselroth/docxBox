// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_HELPER_HELPER_JSON_H_
#define DOCXBOX_HELPER_HELPER_JSON_H_

#include <docxbox/helper/helper_string.h>

#include <string>

#include <vendor/json/single_include/nlohmann/json.hpp>

namespace helper {
namespace Json {

// Extract 1st key out of JSON like: {"<KEY>"...}
extern std::string GetFirstKey(const std::string &json);

extern std::string GetFirstValueOfKey(
    const std::string &json, std::string key);

}  // namespace Json
}  // namespace helper

#endif  // DOCXBOX_HELPER_HELPER_JSON_H_
