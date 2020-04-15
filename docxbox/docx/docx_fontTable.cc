// Copyright (c) 2020 gyselroth GmbH

#include <docxbox/docx/docx_fontTable.h>

docx_fontTable::docx_fontTable() = default;

void docx_fontTable::CollectFontsMetrics(std::string fontTable_xml) {
  u_int32_t xml_len = fontTable_xml.length();
  u_int32_t offset = 0;

  while (offset < xml_len) {
    std::string font_xml = helper::String::GetSubStrBetween(
        fontTable_xml,
        "<w:font ",
        "</w:font",
        offset);

    if (font_xml.empty() || font_xml == "></w:fonts>") break;

    if (font_xml.size() > 3) {
      font_names_.push_back(helper::String::GetSubStrBetween(
          font_xml,
          ":name=\"",
          "\">"));

      alt_names_.push_back(helper::String::GetSubStrBetween(
          font_xml,
          ":altName w:val=\"", "\"/>"));

      charsets_.push_back(helper::String::GetSubStrBetween(
          font_xml,
          ":charset w:val=\"", "\"/>"));

      families_.push_back(helper::String::GetSubStrBetween(
          font_xml,
          ":family w:val=\"", "\"/>"));

      pitches_.push_back(helper::String::GetSubStrBetween(
          font_xml,
          ":pitch w:val=\"", "\"/>"));
    }

    amount_fonts_++;

    offset += font_xml.length() - 1;
  }
}

void docx_fontTable::Clear() {
  font_names_.clear();
  alt_names_.clear();
  charsets_.clear();
  families_.clear();
  pitches_.clear();

  amount_fonts_ = 0;
}

void docx_fontTable::InitCliOutputPadding() {
  max_len_font_name_ = helper::String::GetMaxLength(font_names_);
  max_len_alt_name_ = helper::String::GetMaxLength(alt_names_);
  max_len_charset_ = helper::String::GetMaxLength(charsets_);
  max_len_family_ = helper::String::GetMaxLength(families_);
  max_len_pitch_ = helper::String::GetMaxLength(pitches_);

  if (!helper::Numeric::IsEven(max_len_font_name_)) ++max_len_font_name_;
  if (!helper::Numeric::IsEven(max_len_alt_name_)) ++max_len_alt_name_;
  if (!helper::Numeric::IsEven(max_len_charset_)) ++max_len_charset_;
  if (!helper::Numeric::IsEven(max_len_family_)) ++max_len_family_;
  if (!helper::Numeric::IsEven(max_len_pitch_)) ++max_len_pitch_;

  // Ensure minimum length (= length of rel. headline)
  if (max_len_font_name_ < 5) max_len_font_name_ = 5;
  if (max_len_alt_name_ < 8) max_len_alt_name_ = 8;
  if (max_len_charset_ < 8) max_len_charset_ = 8;
  if (max_len_family_ < 7) max_len_family_ = 7;
  if (max_len_pitch_ < 6) max_len_pitch_ = 6;
}

void docx_fontTable::OutputPlain(const std::string& path_xml_file) {
  InitCliOutputPadding();

  std::cout << path_xml_file << " lists " << amount_fonts_ << " fonts:\n\n";

  std::cout
      << "Font" << helper::String::Repeat(" ", max_len_font_name_ - 4)
      << "    "
      << "AltName" << helper::String::Repeat(" ", max_len_alt_name_ - 7)
      << "     "
      << "CharSet" << helper::String::Repeat(" ", max_len_charset_ - 7)
      << "     "
      << "Family" << helper::String::Repeat(" ", max_len_family_ - 6)
      << "    "
      << "Pitch" << helper::String::Repeat(" ", max_len_pitch_ - 5)
      << "\n";

  std::cout
      << helper::String::Repeat("-", max_len_font_name_) << "    "
      << helper::String::Repeat("-", max_len_alt_name_) << "     "
      << helper::String::Repeat("-", max_len_charset_) << "     "
      << helper::String::Repeat("-", max_len_family_) << "    "
      << helper::String::Repeat("-", max_len_pitch_) << "\n";

  for (int i = 0; i < amount_fonts_; i++) {
    u_int32_t len_font_name = font_names_[i].length();
    u_int32_t len_alt_name = alt_names_[i].length();
    u_int32_t len_charset = charsets_[i].length();
    u_int32_t len_family = families_[i].length();

    std::cout
        << font_names_[i]
        << (len_font_name < max_len_font_name_
          ? helper::String::Repeat(" ", max_len_font_name_ - len_font_name)
          : "") << "    "
        << alt_names_[i]
        << (len_alt_name < max_len_alt_name_
            ? helper::String::Repeat(" ", max_len_alt_name_ - len_alt_name)
            : "") << "     "
        << charsets_[i]
        << (len_charset < max_len_charset_
          ? helper::String::Repeat(" ", max_len_charset_ - len_charset)
          : "") << "     "
        << families_[i]
        << (len_family < max_len_family_
        ? helper::String::Repeat(" ", max_len_family_ - len_family)
        : "") << "    "
        << pitches_[i] << "\n";
  }

  std::cout << "\n";
}

void docx_fontTable::OutputAsJson(const std::string& path_xml_file) {
  std::cout
  << "{"
  << R"("xml": ")" << path_xml_file << "\","
  << "\"fonts\":[";

  for (int i = 0; i < amount_fonts_; i++) {
    std::cout
        << "{"
        << R"("name":")" << font_names_[i] << "\","
        << R"("altName":")" << alt_names_[i] << "\","
        << R"("charset":")" << charsets_[i] << "\","
        << R"("family":")" << families_[i] << "\","
        << R"("pitch":")" << pitches_[i] << "\""
        << "}" << (i <amount_fonts_-1 ? "," : "");
  }

  std::cout
    << "]"
    << "}";
}
