// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/docx/xml/docx_xml_lorem.h>

docx_xml_lorem::docx_xml_lorem(int argc, char **argv) : docx_xml(
    argc,
    argv) {
  unsigned seed = time(nullptr);
  srand(seed);
}

// Replaces all text by random "Lorem Ipsum".
// Note: Segmented text still looks ok:
// surrounding space characters and upper/lower-case of first character
// of each text item is maintained.
bool docx_xml_lorem::RandomizeAllTextInXml(const std::string& path_xml) {
  has_xml_changed_ = false;

  tinyxml2::XMLDocument doc;

  bool is_document_xml = helper::String::EndsWith(path_xml, "document.xml");

  bool is_header_xml = !is_document_xml
                       && helper::String::EndsWith(path_xml, "header1.xml");

  if (!is_document_xml && !is_header_xml) return true;

  std::string xml = helper::File::GetFileContents(path_xml);

  if (is_document_xml) {
    if (!helper::String::Contains(xml, "w:document")
        || !helper::String::Contains(xml, "w:body")) return true;
  } else {
    if (!helper::String::Contains(xml, "w:hdr")) return true;
  }

  doc.LoadFile(path_xml.c_str());

  if (doc.ErrorID() != 0) return false;

  tinyxml2::XMLElement *body;

  body = is_document_xml
    ? doc.FirstChildElement("w:document")->FirstChildElement("w:body")
    : doc.FirstChildElement("w:hdr");

  RandomizeInTextNodes(body);

  return has_xml_changed_
             && tinyxml2::XML_SUCCESS != doc.SaveFile(path_xml.c_str(), true)
         ? docxbox::AppError::Output("Failed saving: " + path_xml)
         : true;
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
            text = RandomizeText(text);

            has_xml_changed_ = true;
          }

          sub_node->SetText(text.c_str());
        }

        continue;
      }
    }

    RandomizeInTextNodes(sub_node);
  } while ((sub_node = sub_node->NextSiblingElement()));
}

// These "segments" might be words,
// but can also be or contain punctuation marks or numbers
std::vector<std::string> docx_xml_lorem::SplitIntoSpaceSeparatedSegments(
    std::string sentence) {

  helper::String::Trim(sentence);

  while (helper::String::Contains(sentence, "  "))
    helper::String::ReplaceAll(sentence, "  ", " ");

  return helper::String::Explode(sentence, ' ');
}

std::string docx_xml_lorem::RandomizeText(std::string str_in) {
  std::vector<std::string> segments_in =
      SplitIntoSpaceSeparatedSegments(str_in);

  u_int32_t amount_segments = segments_in.size();
  u_int32_t index_last_word = amount_segments - 1;

  std::string segment_prev;
  std::string str_out;

  if (str_in[0] == ' ') str_out = " ";

  int i = 0;

  for (auto segment_in : segments_in) {
    std::string word;

    char lead_char_in = segment_in[0];

    if (helper::String::IsNumeric(segment_in, false, true, true)) {
      // Maintain e.g. phone numbers numeric
      word = helper::String::GetRandomNumericString(
          i == 0 && str_in[0] == ' '
            ? segment_in.length() - 1
            : segment_in.length(),
          lead_char_in == '0');
    } else {
      word = GetRandomReplacement(segment_in, segment_prev);
    }

    str_out += word;
    segment_prev = word;

    if (i < index_last_word || str_in[str_in.length() - 1] == ' ')
      // Separate segments by space, maintain overall trailing space
      str_out += ' ';

    i++;
  }

  return str_out;
}

std::string docx_xml_lorem::GetRandomReplacement(
    const std::string &segment_in,
    const std::string &previous_segment) {
  std::string segment;

  do {
    // Get random word of "Lorem Ipsum", prevent repeating a word
    segment = lorem_ipsum_[rand() % 68];
  } while (segment == previous_segment);

  // Maintain lead character which is a punctuation
  if (ispunct(segment_in[0])) {
    segment = segment_in[0] + segment;
  } else {
    if (isalpha(segment[0])) {
      // Maintain capitalization of segment
      if (helper::String::IsAllUpper(segment_in)) {
        segment = helper::String::ToUpper(segment);
      } else {
        segment[0] = isupper(segment_in[0])
            ? toupper(segment[0])
            : tolower(segment[0]);
      }
    }
  }

  // Maintain trailing punctuation
  char ch_trail_original = segment_in[ segment_in.length() - 1];

  if (ispunct(ch_trail_original)) segment += ch_trail_original;

  return segment;
}
