// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/helper/helper_string.h>

#include <iostream>
#include <iterator>
#include <regex>  // NOLINT [build/c++11]
#include <utility>

namespace helper {

// Check whether given string starts w/ given prefix
bool String::StartsWith(const char *str, const char *prefix) {
  return 0 == strncmp(str, prefix, strlen(prefix));
}

bool String::StartsNumeric(const char *str) {
  return isdigit(str[0]);
}

// Check whether given string ends w/ given string
bool String::EndsWith(std::string const &str, std::string const &ending) {
  return ending.empty()
      || (ending.size() <= str.size()
          && std::equal(ending.rbegin(), ending.rend(), str.rbegin()));
}

bool String::EndsWithCaseInsensitive(const char *str,
                                     const std::string &ending) {
  return EndsWith(ToLower(str), ending);
}

bool String::EndsWithCaseInsensitive(std::string str,
                                     const std::string &ending) {
  return EndsWith(ToLower(std::move(str)), ending);
}

bool String::IsAnyOf(
    const std::string& str,
    std::vector<std::string> endings) {
  uint16_t amount_endings = endings.size();

  for (int i = 0; i < amount_endings; ++i) {
    if (str == endings[i]) return true;
  }

  return false;
}

bool String::Matches(const std::string &subject, const std::string &pattern) {
  bool str_empty = subject.empty();
  bool pattern_empty = subject.empty();

  if (str_empty != pattern_empty) return false;

  if (str_empty && pattern_empty) return true;

  return std::regex_match(subject, std::regex(pattern));
}

bool String::Contains(const std::string &haystack, const char *needle) {
  return std::string::npos != haystack.find(needle);
}

bool String::IsWhiteSpace(const std::string &str) {
  for (char c : str) if (c != ' ') return false;

  return true;
}

bool String::Replace(
    std::string *haystack, const char *needle, const char *replacement) {
  size_t needle_len = strlen(needle);

  size_t index = 0;
  index = (*haystack).find(needle, index);

  if (std::string::npos == index) return false;

  (*haystack).replace(index, needle_len, replacement);

  return true;
}

void String::Remove(std::string *haystack, const std::regex &pattern) {
  *haystack = std::regex_replace(*haystack, pattern, "");
}

int String::RemoveAll(std::string *haystack,
                       const std::string &needle) {
  return ReplaceAll(haystack, needle, "");
}

int String::ReplaceAll(std::string *haystack,
                       const std::string &needle,
                       const std::string &replacement) {
  // Get first occurrence
  size_t pos = (*haystack).find(needle);

  int amount_replaced = 0;

  // Repeat till end is reached
  while (pos != std::string::npos) {
    // Replace this occurrence of Sub String
    (*haystack).replace(pos, needle.size(), replacement);

    // Get the next occurrence from the current position
    pos = (*haystack).find(needle, pos + replacement.size());

    amount_replaced++;
  }

  return amount_replaced;
}

// Get sub string between given surrounding left- and right-hand-side delimiters
// return std::string  The enclosed sub-string or an empty string
std::string String::GetSubStrBetween(const std::string &str,
                                     const char *lhs,
                                     const char *rhs,
                                     u_int32_t *offset) {
  size_t offsetStart = str.find(lhs, *offset);

  if (std::string::npos == offsetStart) return "";

  size_t offsetEnd = str.find(rhs, offsetStart);

  if (std::string::npos == offsetEnd) return "";

  // Exclude LHS
  offsetStart += strlen(lhs);

  *offset = offsetStart;

  return str.substr(offsetStart, offsetEnd - offsetStart);
}

std::string String::GetSubStrBetween(const std::string &str,
                                     const char *lhs,
                                     const char *rhs) {
    u_int32_t offset = 0;

  return GetSubStrBetween(str, lhs, rhs, &offset);
}

int String::OffsetChar(const std::string &str, char c, int offset) {
  int16_t len = str.length();

  do {
    if (str[offset] == c) return offset;

    ++offset;
  } while (offset < len);

  return -1;
}

// Split given string by given character delimiter into vector of strings
std::vector<std::string> String::Explode(std::string const &str,
                                         char delimiter) {
  std::vector<std::string> result;
  std::istringstream iss(str);

  for (std::string token; std::getline(iss, token, delimiter);)
    result.push_back(std::move(token));

  return result;
}

std::string String::GetTrailingWord(std::string *str) {
  if ((*str).empty() || !Contains(*str, " ")) return *str;

  Trim(str);

  while (Contains(*str, "  ")) ReplaceAll(str, "  ", " ");

  auto words = Explode(*str, ' ');

  return words[words.size() - 1];
}

extern std::string String::ExtractTrailingNumber(
    std::string str, std::string default_if_none) {
  auto offset_end =  str.find_last_of('.');

  if (offset_end == std::string::npos) return default_if_none;

  int offset_start = -1;

  bool was_numeric = false;

  for (uint16_t i = str.length() - 1; i > 0; i--) {
    if (isdigit(str[i])) {
      was_numeric = true;
    } else {
      if (was_numeric) {
        offset_start = i + 1;
        break;
      }
    }
  }

  return -1 == offset_start
    ? default_if_none
    : str.substr(offset_start, offset_end - offset_start);
}

std::string String::ExtractLeadingNumber(std::string str) {
  std::string number;

  for (uint16_t i = 0; i < str.length(); i++) {
    if (!isdigit(str[i])) return number;

    number += str[i];
  }

  return number;
}

std::string String::Implode(
    std::vector<std::string> strings,
    const char* const delimiter) {
  std::ostringstream imploded;

  std::copy(
      strings.begin(),
      strings.end(),
      std::ostream_iterator<std::string>(imploded, delimiter));

  return imploded.str();
}

std::string String::RenderTwoColumns(
    const std::string& col_1, const std::string& col_2, int amount_gap) {
  std::vector<std::string> lines_1 = Explode(col_1, '\n');
  std::vector<std::string> lines_2 = Explode(col_2, '\n');

  std::string gap = Repeat(" ", amount_gap);

  int len_longest_line = GetMaxLength(lines_1);

  std::string out;

  int amount_lines_2 = lines_2.size();

  int index = 0;

  for (const auto& line : lines_1) {
    out += line;

    uint16_t len_line_1 = line.length();

    if (len_line_1 < len_longest_line)
      out += Repeat(" ", len_longest_line - len_line_1);

    out += gap;

    if (index < amount_lines_2
        && !lines_2[index].empty()) out.append(lines_2[index]);

    out += "\n";

    ++index;
  }

  return out;
}

// Trim from start (in place)
void String::LTrim(std::string *s) {
  (*s).erase(
      (*s).begin(),
      std::find_if(
          (*s).begin(),
          (*s).end(),
          std::not1(std::ptr_fun<int, int>(std::isspace))));
}

// Trim from end (in place)
void String::RTrim(std::string *s) {
  (*s).erase(
      std::find_if(
          (*s).rbegin(),
          (*s).rend(),
          std::not1(std::ptr_fun<int, int>(std::isspace)))
          .base(),
      (*s).end());
}

// Trim from both ends (in place)
void String::Trim(std::string *s) {
  LTrim(s);
  RTrim(s);
}

extern bool String::IsNumeric(
    std::string *str,
    bool trim,
    bool can_contain_punctuation,
    bool can_contain_spaces) {
  if ((*str).empty()) return false;

  if (trim) Trim(str);

  if ((*str).empty() && !can_contain_spaces) return false;

  for (char i : *str) {
    if ((can_contain_spaces && i == ' ')
        || (can_contain_punctuation && ispunct(i))) continue;

    if (!isdigit(i)) return false;
  }

  return true;
}

bool String::IsAllUpper(const std::string& str) {
  for (auto c : str)
    if (isalpha(c) && !isupper(c)) return false;

  return true;
}

std::string String::ToLower(std::string str) {
  std::transform(
      str.begin(),
      str.end(),
      str.begin(),
      [](unsigned char c) {
        return std::tolower(c);
      });

  return str;
}

std::string String::ToUpper(const std::string& str) {
  std::string upper;

  for (auto c : str) upper += toupper(c);

  return upper;
}

u_int32_t String::GetMaxLength(const std::vector<std::string>& strings) {
  u_int32_t max = 0;

  for (const auto& str : strings) {
    u_int32_t len = str.length();

    if (len > max) max = len;
  }

  return max;
}

u_int32_t String::GetMaxLineLength(const std::vector<std::string>& str_1,
                                   const std::vector<std::string>& str_2) {
  int max_1 = GetMaxLength(str_1);
  int max_2 = GetMaxLength(str_2);

  return max_1 > max_2 ? max_1 : max_2;
}

std::string String::Repeat(const std::string& str, u_int16_t amount) {
  std::string out;

  for (u_int16_t i = 0; i < amount; i++) {
    out+= str;
  }

  return out;
}

std::string String::GetRandomNumericString(
    u_int32_t length,
    bool starts_with_zero) {
  std::string str = starts_with_zero
    ? "0"
    : std::to_string(std::rand() % 9);

  if (length == 1) return str;

  while (str.length() < length) str += std::to_string(std::rand() % 9);

  return str;
}

}  // namespace helper
