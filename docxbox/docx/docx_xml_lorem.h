// Copyright (c) 2020 gyselroth GmbH

#ifndef DOCXBOX_DOCX_DOCX_XML_LOREM_H_
#define DOCXBOX_DOCX_DOCX_XML_LOREM_H_

#include <iostream>
#include <string>

#include "docx_xml.h"
#include "../helper/helper_file.h"

#include "../helper/helper_string.h"
#include "../../vendor/tinyxml2/tinyxml2.h"

class docx_xml_lorem:docx_xml {
 public:
  docx_xml_lorem(int argc, char **argv);

  bool RandomizeAllTextInXml(const std::string& path_xml);

 private:
  char const* lorem_ipsum_[69] = {
      "lorem", "ipsum", "dolor", "sit", "amet,", "consectetur", "adipiscing",
      "elit,", "sed", "do", "eiusmod", "tempor", "incididunt", "ut", "labore",
      "et", "dolore", "magna", "aliqua.", "ut", "enim", "ad", "minim",
      "veniam,", "quis", "nostrud", "exercitation", "ullamco", "laboris",
      "nisi", "ut", "aliquip", "ex", "ea", "commodo", "consequat.", "duis",
      "aute", "irure", "dolor", "in", "reprehenderit", "in", "voluptate",
      "velit", "esse", "cillum", "dolore", "eu", "fugiat", "nulla", "pariatur.",
      "excepteur", "sint", "occaecat", "cupidatat", "non", "proident,", "sunt",
      "in", "culpa", "qui", "officia", "deserunt", "mollit", "anim", "id",
      "est", "laborum."
  };

  void RandomizeInTextNodes(tinyxml2::XMLElement *node);

  std::string GetRandomText(
      int amount_words,
      bool starts_with_space,
      bool start_uppercase,
      bool end_with_dot,
      bool is_numeric,
      bool first_number_is_zero,
      int amount_characters);

  std::string GetRandomNumber(
      u_int32_t amount_characters,
      bool first_number_is_zero);
};

#endif  // DOCXBOX_DOCX_DOCX_XML_LOREM_H_
