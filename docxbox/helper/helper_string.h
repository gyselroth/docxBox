// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_HELPER_HELPER_STRING_H_
#define DOCXBOX_HELPER_HELPER_STRING_H_

#include <docxbox/helper/helper_numeric.h>

#include <fcntl.h>
#include <sys/sendfile.h>

#include <algorithm>
#include <codecvt>
#include <cstring>
#include <cmath>
#include <functional>
#include <locale>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

static const char *const kAnsiBold = "\033[1m";
static const char *const kAnsiDim = "\033[2m";
static const char *const kAnsiItalic = "\033[3m";
static const char *const kAnsiUnderline = "\033[4m";
static const char *const kAnsiReverse = "\033[7m";
static const char *const kAnsiReset = "\033[0m";

static const char *const kAnsiBlack = "\033[0;30m";
static const char *const kAnsiBlue = "\033[0;34m";
static const char *const kAnsiCyan = "\033[0;36m";
static const char *const kAnsiDarkGray = "\033[1;30m";
static const char *const kAnsiGreen = "\033[0;32m";
static const char *const kAnsiLightBlue = "\033[1;34m";
static const char *const kAnsiLightCyan = "\033[1;36m";
static const char *const kAnsiLightGray = "\033[0;37m";
static const char *const kAnsiLightGreen = "\033[1;32m";
static const char *const kAnsiLightPurple = "\033[1;35m";
static const char *const kAnsiLightRed = "\033[1;31m";
static const char *const kAnsiOrange = "\033[0;33m";
static const char *const kAnsiPurple = "\033[0;35m";
static const char *const kAnsiRed = "\033[0;31m";
static const char *const kAnsiWhite = "\033[1;37m";
static const char *const kAnsiYellow = "\033[1;33m";

namespace helper {
namespace String {

// Check whether given string ends w/ given string
bool EndsWith(std::string const &value, std::string const &ending);
// Check whether given string starts w/ given prefix
extern bool StartsWith(const char *str, const char *prefix);

extern bool StartsNumeric(const char *str);

extern bool IsAnyOf(const std::string& str, std::vector<std::string> endings);

extern bool Contains(const std::string &haystack, const char *needle);

extern bool IsWhiteSpace(const std::string &str);

extern bool Replace(
    std::string &haystack, const char *needle, const char *replacement);

extern int ReplaceAll(
    std::string &haystack,
    const std::string& needle,
    const std::string& replacement);

// Get substring between given surrounding left- and center-hand-side delimiters
extern std::string GetSubStrBetween(
    const std::string &str,
    const char *lhs,
    const char *rhs,
    u_int32_t &offset);

extern std::string GetSubStrBetween(
    const std::string &str,
    const char *lhs,
    const char *rhs);

extern int OffsetChar(const std::string &str, char c, int offset = 0);

// Split given string by given character delimiter into vector of strings
extern std::vector<std::string> Explode(std::string const &str, char delimiter);

extern std::string GetTrailingWord(std::string str);
extern std::string ExtractRightMostNumber(
    std::string str, std::string default_if_none = "1");

extern std::string Implode(
    std::vector<std::string> strings, const char* delimiter = ", ");

extern std::string RenderTwoColumns(
    std::string col_1, std::string col_2, int amount_gap = 4);

extern void LTrim(std::string &s);
extern void RTrim(std::string &s);
extern void Trim(std::string &s);

extern bool IsNumeric(
    std::string str,
    bool trim,
    bool can_contain_punctuation,
    bool can_contain_spaces);

extern bool IsAllUpper(const std::string& str);

extern std::string ToLower(std::string str);
extern std::string ToUpper(const std::string &str);

extern u_int32_t GetMaxLength(const std::vector<std::string>& str_1);

extern u_int32_t GetMaxLineLength(const std::vector<std::string>& str_1,
                                  const std::vector<std::string>& str_2);

extern std::string Repeat(const std::string& str, u_int16_t amount);

extern std::string GetRandomNumericString(
    u_int32_t length,
    bool starts_with_zero = false);

}  // namespace String
}  // namespace helper

#endif  // DOCXBOX_HELPER_HELPER_STRING_H_
