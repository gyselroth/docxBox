// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_HELPER_HELPER_JSON_H_
#define DOCXBOX_HELPER_HELPER_JSON_H_

#include <docxbox/helper/helper_string.h>

#include <string>

#include <vendor/json/single_include/nlohmann/json.hpp>

namespace helper {
namespace Json {

extern bool IsJson(const std::string &str, bool contains_association = true);

// Extract 1st key out of JSON like: {"<KEY>"...}
extern std::string GetFirstKey(const std::string &json);

extern std::string GetFirstValueOfKey(
    const std::string &json, const std::string& key);

extern int GetAmountItems(const std::string& json);

}  // namespace Json
}  // namespace helper

#endif  // DOCXBOX_HELPER_HELPER_JSON_H_
