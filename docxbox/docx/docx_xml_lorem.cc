// Copyright (c) 2020 gyselroth GmbH

#include "docx_xml_lorem.h"

docx_xml_lorem::docx_xml_lorem(int argc, char **argv) : docx_xml(
    argc,
    argv) {
  unsigned seed = time(nullptr);
  srand(seed);
}

// Replaces all text by random "Lorem Ipsum".
// Note: Segmented text still looks ok: as upper/lower-case of first character
// of each text item is maintained.
bool docx_xml_lorem::RandomizeAllTextInXml(const std::string& path_xml) {
  tinyxml2::XMLDocument doc;

  std::string kPathXml = path_xml;
  std::string xml = helper::File::GetFileContents(kPathXml);

  if (!helper::String::Contains(xml, "w:document")
      || !helper::String::Contains(xml, "w:body")) return true;

  doc.LoadFile(path_xml.c_str());

  if (doc.ErrorID() != 0) return false;

  tinyxml2::XMLElement *body = doc.FirstChildElement("w:document")
      ->FirstChildElement("w:body");

  RandomizeInTextNodes(body);

  if (tinyxml2::XML_SUCCESS != doc.SaveFile(path_xml.c_str(), true)) {
    std::cout << "Error - Failed saving: " << path_xml << "\n";

    return false;
  }

  return true;
}

// Replaces the searched string when contained within a single <w:t> node
void docx_xml_lorem::RandomizeInTextNodes(tinyxml2::XMLElement *node) {
  if (!node || node->NoChildren()) return;

  tinyxml2::XMLElement *sub_node = node->FirstChildElement();

  if (sub_node == nullptr) return;

  do {
    if (!sub_node) continue;

    const char *value = sub_node->Value();

    if (value) {
      if (0 == strcmp(value, "w:t")
          && sub_node->FirstChild() != nullptr) {
        std::string text = sub_node->GetText();

        if (!text.empty()) {
          u_int32_t text_length = text.length();

          if (text_length > 1) {
            bool starts_with_space = text[0] == ' ';
            bool is_numeric = helper::String::IsNumeric(text, true, true);

            text = GetRandomText(
                helper::String::GetAmountWords(text),
                starts_with_space,
                isupper(text[starts_with_space && text_length > 1 ? 1 : 0]),
                helper::String::EndsWith(text, "."),
                is_numeric,
                is_numeric && text[starts_with_space ? 1 : 0] == '0',
                text_length);
          }

          sub_node->SetText(text.c_str());
        }

        continue;
      }
    }

    RandomizeInTextNodes(sub_node);
  } while ((sub_node = sub_node->NextSiblingElement()));
}

std::string docx_xml_lorem::GetRandomText(
    int amount_words,
    bool starts_with_space,
    bool start_uppercase,
    bool end_with_dot,
    bool is_numeric,
    bool first_number_is_zero,
    int amount_characters) {
  std::string str;

  if (starts_with_space) str = " ";

  if (is_numeric) {
    // Maintain e.g. phone numbers numeric
    return GetRandomNumber(
        starts_with_space ? amount_characters - 1 : amount_characters,
        first_number_is_zero);
  }

  bool make_upper = start_uppercase;

  unsigned int offset_word_start = 0;

  std::string previous_word;

  u_int32_t index_last_word = amount_words - 1;
  u_int32_t index_second_last_word = index_last_word - 1;

  for (int i = 0; i < amount_words; i++) {
    char *word;

    do {
      word = const_cast<char *>(lorem_ipsum_[std::rand() % 68]);
    } while (word == previous_word);

    if (i == index_second_last_word
      && (helper::String::EndsWith(word, ",")
          || helper::String::EndsWith(word, "."))) {
      word = helper::String::RemoveLastCharacter(word);
    }

    str += word;

    previous_word = word;

    if (i < index_last_word) str += ' ';

    if (make_upper) str[offset_word_start] = toupper(str[offset_word_start]);

    offset_word_start = str.length();

    make_upper = helper::String::EndsWith(str, ". ");
  }

  if (helper::String::EndsWith(str, ","))
    str = str.substr(0, str.length() - 1);

  return end_with_dot && !helper::String::EndsWith(str, ".")
    ? str + "."
    : str;
}

std::string docx_xml_lorem::GetRandomNumber(
    u_int32_t amount_characters,
    bool first_number_is_zero) {
  std::string str;

  if (first_number_is_zero) {
    str = "0";
    amount_characters--;
  }

  for (u_int32_t i = 0; i < amount_characters; i++) {
    str += std::to_string(std::rand() % 9);
  }

  return str;
}
