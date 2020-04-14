// Copyright (c) 2020 gyselroth GmbH

#ifndef DOCXBOX_HELPER_HELPER_STRING_H_
#define DOCXBOX_HELPER_HELPER_STRING_H_

#include <algorithm>
#include <codecvt>
#include <cstring>
#include <functional>
#include <locale>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "helper_numeric.h"

namespace helper {
namespace String {

// Check whether given string ends w/ given string
bool EndsWith(std::string const &value, std::string const &ending);
// Check whether given string starts w/ given prefix
extern bool StartsWith(const char *str, const char *prefix);

extern bool Contains(std::string &haystack, const char *needle);

extern std::string Replace(
    std::string &haystack,
    const char *needle,
    const char *replacement);

extern int ReplaceAll(
    std::string &haystack,
    const std::string& needle,
    const std::string& replacement);

// Get substring between given surrounding left- and center-hand-side delimiters
extern std::string GetSubStrBetween(
    std::string &str,
    const char *lhs,
    const char *rhs,
    u_int32_t &offset);

extern std::string GetSubStrBetween(
    std::string &str,
    const char *lhs,
    const char *rhs);

// Split given string by given character delimiter into vector of strings
extern std::vector<std::string> Explode(std::string const &str, char delimiter);

extern void LTrim(std::string &s);
extern void RTrim(std::string &s);
extern void Trim(std::string &s);

extern bool IsNumeric(
    std::string str,
    bool trim,
    bool can_contain_punctuation,
    bool can_contain_spaces);

extern bool IsAllUpper(const std::string& str);

extern std::string ToUpper(const std::string &str);

extern u_int32_t GetMaxLength(const std::vector<std::string>& strings);

extern std::string Repeat(const std::string& str, u_int16_t amount);

extern std::string GetRandomNumericString(u_int32_t length, bool starts_with_zero);

}  // namespace String
}  // namespace helper

#endif  // DOCXBOX_HELPER_HELPER_STRING_H_
