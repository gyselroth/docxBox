
#include <cmath>
#include "helper_string.h"

namespace helper {

/**
 * Check whether given string starts w/ given prefix
 */
bool String::StartsWith(const char *str, const char *prefix) {
  return 0==strncmp(str, prefix, strlen(prefix));
}

/**
 * Check whether given string ends w/ given string
 */
bool String::EndsWith(std::string const &value, std::string const &ending) {
  return ending.size() <= value.size() && std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

bool String::Contains(std::string &haystack, const char *needle) {
  return std::string::npos!=haystack.find(needle);
}

/**
 * Get amount of sub string occurrences
 */
int String::GetSubStrCount(const char *str, const char *sub) {
  auto length = static_cast<int>(strlen(sub));
  if (length==0) return 0;

  int count = 0;
  for (str = strstr(str, sub); str; str = strstr(str + length, sub)) ++count;

  return count;
}

std::string String::ReplaceFirstOccurrence(
    const char *needle, const char *replacement, std::string &haystack) {
  size_t needle_len = strlen(needle);
  size_t index = haystack.find(needle, 0);

  if (std::string::npos!=index)
    haystack.replace(index, needle_len, replacement);

  return haystack;
}


std::string String::Replace(std::string &haystack, const char *needle, const char *replacement) {
  size_t needle_len = strlen(needle);

  size_t index = 0;
  index = haystack.find(needle, index);
  if (std::string::npos!=index) haystack.replace(index, needle_len, replacement);

  return haystack;
}

std::string String::ReplaceAll(std::string &haystack, const char *needle, const char *replacement) {
  size_t needle_len = strlen(needle);

  size_t index = 0;
  while (true) {
    index = haystack.find(needle, index);
    if (std::string::npos==index) break;

    haystack.replace(index, needle_len, replacement);
  }
  // TODO ensure avoid endless looping (when e.g. replacing "a" by "aa")
  return haystack;
}

/**
 *  Get sub string between given surrounding left- and right-hand-side delimiters
 *
 *  return std::string  The enclosed sub-string or an empty string
 */
std::string String::GetSubStrBetween(std::string &str, const char *lhs, const char *rhs, unsigned long &offset) {
  size_t offsetStart = str.find(lhs, offset);

  if (std::string::npos==offsetStart) return "";

  size_t offsetEnd = str.find(rhs, offsetStart);

  if (std::string::npos==offsetEnd) return "";

  // Exclude LHS
  offsetStart += strlen(lhs);

  offset = offsetStart;

  return str.substr(offsetStart,offsetEnd - offsetStart);
}

std::string String::GetSubStrBetween(std::string &str, const char *lhs, const char *rhs) {
  unsigned long offset = 0;

  return GetSubStrBetween(str, lhs, rhs, offset);
}

std::string String::GetSubStrAfter(std::string &str, const char *delimiter, unsigned long pos) {
  size_t offsetStart = str.find(delimiter, pos);

  return std::string::npos!=offsetStart
         ? str.substr(offsetStart+strlen(delimiter))
         : "";
}

/**
 * Split given string by given character delimiter into vector of strings
 */
std::vector<std::string> String::Explode(std::string const &str, char delimiter) {
  std::vector<std::string> result;
  std::istringstream iss(str);

  for (std::string token; std::getline(iss, token, delimiter);)
    result.push_back(std::move(token));

  return result;
}

bool String::IsNumeric(char c) {
  return c >= 48 && c <= 57;
}

/**
 * Trim from start (in place)
 */
void String::LTrim(std::string &s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(),
                                  std::not1(std::ptr_fun<int, int>(std::isspace))));
}

/**
 * Trim from end (in place)
 */
void String::RTrim(std::string &s) {
  s.erase(std::find_if(
      s.rbegin(),
      s.rend(),
      std::not1(std::ptr_fun<int, int>(std::isspace))).base(),
          s.end()
  );
}

/**
 * Trim from both ends (in place)
 */
void String::Trim(std::string &s) {
  LTrim(s);
  RTrim(s);
}

std::string String::ToLower(std::string s) {
  std::transform(s.begin(), s.end(), s.begin(),
                 [](unsigned char c) { return std::tolower(c); }
  );

  return s;
}

std::string String::ToUpper(std::string s) {

  return s;
}

std::string String::UcFirst(std::string s) {
  s[0] = static_cast<char>(std::toupper(s[0]));

  return s;
}

unsigned long String::GetMaxLength(const std::vector<std::string>& strings) {
  unsigned long max = 0;
  
  for (const auto& str : strings) {
    unsigned long len = str.length();

    if (len > max) max = len;
  }

  return max;
}

std::string String::Repeat(const std::string& str, unsigned long amount) {
  std::string out;

  for (unsigned long i=0; i<amount; i++) {
    out+= str;
  }

  return out;
}

} // namespace helper