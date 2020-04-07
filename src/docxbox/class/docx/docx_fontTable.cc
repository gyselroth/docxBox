
#include "docx_fontTable.h"

docx_fontTable::docx_fontTable() = default;

void docx_fontTable::CollectFontsMetrics(std::string fontTable_xml) {
  unsigned long xml_len = fontTable_xml.length();
  unsigned long offset = 0;

  while (offset < xml_len) {
    std::string font_xml = helper::String::GetSubStrBetween(
        fontTable_xml,
        "<w:font ",
        "</w:font",
        offset
    );

    if (font_xml.empty() || font_xml == "></w:fonts>") break;

    if (font_xml.size() > 3) {
      font_names.push_back(helper::String::GetSubStrBetween(font_xml, ":name=\"", "\">"));
      alt_names.push_back(helper::String::GetSubStrBetween(font_xml, ":altName w:val=\"", "\"/>"));
      charsets.push_back(helper::String::GetSubStrBetween(font_xml, ":charset w:val=\"", "\"/>"));
      families.push_back(helper::String::GetSubStrBetween(font_xml, ":family w:val=\"", "\"/>"));
      pitches.push_back(helper::String::GetSubStrBetween(font_xml, ":pitch w:val=\"", "\"/>"));
    }

    amount_fonts++;

    offset += font_xml.length() - 1;
  }
}

void docx_fontTable::Clear() {
  font_names.clear();
  alt_names.clear();
  charsets.clear();
  families.clear();
  pitches.clear();

  amount_fonts = 0;
}

void docx_fontTable::InitCliOutputPadding() {
  max_len_font_name = helper::String::GetMaxLength(font_names);
  max_len_alt_name = helper::String::GetMaxLength(alt_names);
  max_len_charset = helper::String::GetMaxLength(charsets);
  max_len_family = helper::String::GetMaxLength(families);
  max_len_pitch = helper::String::GetMaxLength(pitches);

  if (!helper::Numeric::IsEven(max_len_font_name)) ++max_len_font_name;
  if (!helper::Numeric::IsEven(max_len_alt_name)) ++max_len_alt_name;
  if (!helper::Numeric::IsEven(max_len_charset)) ++max_len_charset;
  if (!helper::Numeric::IsEven(max_len_family)) ++max_len_family;
  if (!helper::Numeric::IsEven(max_len_pitch)) ++max_len_pitch;

  // Ensure minimum length (= length of rel. headline)
  if (max_len_font_name < 5) max_len_font_name = 5;
  if (max_len_alt_name < 8) max_len_alt_name = 8;
  if (max_len_charset < 8) max_len_charset = 8;
  if (max_len_family < 7) max_len_family = 7;
  if (max_len_pitch < 6) max_len_pitch = 6;
}

void docx_fontTable::OutputPlain(const std::string& path_xml_file) {
  InitCliOutputPadding();

  std::cout << path_xml_file << " lists " << amount_fonts << " fonts:\n\n";

  std::cout
      << "Font" << helper::String::Repeat(" ", max_len_font_name - 4) << "    "
      << "AltName" << helper::String::Repeat(" ", max_len_alt_name - 7) << "     "
      << "CharSet" << helper::String::Repeat(" ", max_len_charset - 7) << "     "
      << "Family" << helper::String::Repeat(" ", max_len_family - 6) << "    "
      << "Pitch" << helper::String::Repeat(" ", max_len_pitch - 5) << "\n";

  std::cout
      << helper::String::Repeat("-", max_len_font_name) << "    "
      << helper::String::Repeat("-", max_len_alt_name) << "     "
      << helper::String::Repeat("-", max_len_charset) << "     "
      << helper::String::Repeat("-", max_len_family) << "    "
      << helper::String::Repeat("-", max_len_pitch) << "\n";

  for (int i=0; i<amount_fonts; i++) {
    unsigned long len_font_name = font_names[i].length();
    unsigned long len_alt_name = alt_names[i].length();
    unsigned long len_charset = charsets[i].length();
    unsigned long len_family = families[i].length();

    std::cout
      << font_names[i]
      << (len_font_name < max_len_font_name
        ? helper::String::Repeat(" ", max_len_font_name - len_font_name)
        : ""
        ) << "    "
      << alt_names[i]
      << (len_alt_name < max_len_alt_name
            ? helper::String::Repeat(" ", max_len_alt_name - len_alt_name)
            : ""
        ) << "     "
      << charsets[i]
      << (len_charset < max_len_charset
        ? helper::String::Repeat(" ", max_len_charset - len_charset)
        : ""
      ) << "     "
      << families[i]
      << (len_family < max_len_family
        ? helper::String::Repeat(" ", max_len_family - len_family)
        : ""
      ) << "    "
      << pitches[i] << "\n";
  }

  std::cout << "\n";
}

void docx_fontTable::OutputAsJson(const std::string& path_xml_file) {
  std::cout
  << "{"
  << R"("xml": ")" << path_xml_file << "\","
  << "\"fonts\":[" ;

  for (int i=0; i<amount_fonts; i++) {
    std::cout
        << "{"
        << R"("name":")" << font_names[i] << "\","
        << R"("altName":")" << alt_names[i] << "\","
        << R"("charset":")" << charsets[i] << "\","
        << R"("family":")" << families[i] << "\","
        << R"("pitch":")" << pitches[i] << "\""
        << "}" << (i <amount_fonts-1 ? "," : "");
  }

  std::cout
    << "]"
    << "}";
}
