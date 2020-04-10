// Copyright (c) 2020 gyselroth GmbH

#include <cmath>
#include "helper_string.h"

namespace helper {

/**
 * Check whether given string starts w/ given prefix
 */
bool String::StartsWith(const char *str, const char *prefix) {
  return 0 == strncmp(str, prefix, strlen(prefix));
}

/**
 * Check whether given string ends w/ given string
 */
bool String::EndsWith(std::string const &value, std::string const &ending) {
  return ending.size() <= value.size()
      && std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

bool String::Contains(std::string &haystack, const char *needle) {
  return std::string::npos != haystack.find(needle);
}

std::string String::Replace(
    std::string &haystack,
    const char *needle,
    const char *replacement) {
  size_t needle_len = strlen(needle);

  size_t index = 0;
  index = haystack.find(needle, index);

  if (std::string::npos != index)
    haystack.replace(index, needle_len, replacement);

  return haystack;
}

int String::ReplaceAll(
    std::string &data,
    const std::string &toSearch,
    const std::string &replaceStr) {
  // Get first occurrence
  size_t pos = data.find(toSearch);

  int amount_replaced = 0;

  // Repeat till end is reached
  while (pos != std::string::npos) {
    // Replace this occurrence of Sub String
    data.replace(pos, toSearch.size(), replaceStr);

    // Get the next occurrence from the current position
    pos = data.find(toSearch, pos + replaceStr.size());

    amount_replaced++;
  }

  return amount_replaced;
}

/**
 *  Get sub string between given surrounding left- and right-hand-side delimiters
 *
 *  return std::string  The enclosed sub-string or an empty string
 */
std::string String::GetSubStrBetween(
    std::string &str,
    const char *lhs,
    const char *rhs,
    u_int32_t &offset) {
  size_t offsetStart = str.find(lhs, offset);

  if (std::string::npos == offsetStart) return "";

  size_t offsetEnd = str.find(rhs, offsetStart);

  if (std::string::npos == offsetEnd) return "";

  // Exclude LHS
  offsetStart += strlen(lhs);

  offset = offsetStart;

  return str.substr(offsetStart, offsetEnd - offsetStart);
}

std::string String::GetSubStrBetween(
    std::string &str,
    const char *lhs,
    const char *rhs) {
    u_int32_t offset = 0;

  return GetSubStrBetween(str, lhs, rhs, offset);
}

/**
 * Split given string by given character delimiter into vector of strings
 */
std::vector<std::string> String::Explode(
    std::string const &str,
    char delimiter) {
  std::vector<std::string> result;
  std::istringstream iss(str);

  for (std::string token; std::getline(iss, token, delimiter);)
    result.push_back(std::move(token));

  return result;
}

/**
 * Trim from start (in place)
 */
void String::LTrim(std::string &s) {
  s.erase(
      s.begin(),
      std::find_if(
          s.begin(),
          s.end(),
          std::not1(std::ptr_fun<int, int>(std::isspace))));
}

/**
 * Trim from end (in place)
 */
void String::RTrim(std::string &s) {
  s.erase(
      std::find_if(
          s.rbegin(),
          s.rend(),
          std::not1(std::ptr_fun<int, int>(std::isspace)))
          .base(),
      s.end());
}

/**
 * Trim from both ends (in place)
 */
void String::Trim(std::string &s) {
  LTrim(s);
  RTrim(s);
}

u_int32_t String::GetMaxLength(const std::vector<std::string>& strings) {
  u_int32_t max = 0;

  for (const auto& str : strings) {
    u_int32_t len = str.length();

    if (len > max) max = len;
  }

  return max;
}

std::string String::Repeat(const std::string& str, u_int16_t amount) {
  std::string out;

  for (u_int16_t i = 0; i < amount; i++) {
    out+= str;
  }

  return out;
}

}  // namespace helper
