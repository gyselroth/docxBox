// Copyright (c) 2020 gyselroth GmbH

#ifndef CLASS_HELPER_STRING_H
#define CLASS_HELPER_STRING_H

#include <algorithm>
#include <codecvt>
#include <cstring>
#include <locale>
#include <sstream>
#include <string>
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
    std::string &data,
    const std::string& toSearch,
    const std::string& replaceStr);

// Get sub string between given surrounding left- and center-hand-side delimiters
extern std::string GetSubStrBetween(
    std::string &str,
    const char *lhs,
    const char *rhs,
    unsigned long &offset);

extern std::string GetSubStrBetween(
    std::string &str,
    const char *lhs,
    const char *rhs);

// Split given string by given character delimiter into vector of strings
extern std::vector<std::string> Explode(std::string const &str, char delimiter);

extern void LTrim(std::string &s);
extern void RTrim(std::string &s);
extern void Trim(std::string &s);

extern unsigned long GetMaxLength(const std::vector<std::string>& strings);

extern std::string Repeat(const std::string& str, unsigned long amount);

} // namespace String
} // namespace helper

#endif //CLASS_HELPER_STRING_H