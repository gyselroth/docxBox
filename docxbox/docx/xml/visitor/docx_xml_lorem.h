// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_DOCX_XML_VISITOR_DOCX_XML_LOREM_H_
#define DOCXBOX_DOCX_XML_VISITOR_DOCX_XML_LOREM_H_

#include <docxbox/app/app_log.h>
#include <docxbox/docx/xml/docx_xml.h>
#include <docxbox/helper/helper_file.h>
#include <docxbox/helper/helper_string.h>

#include <vendor/tinyxml2/tinyxml2.h>

#include <iostream>
#include <string>
#include <vector>

class docx_xml_lorem:docx_xml {
 public:
  docx_xml_lorem(int argc, const std::vector<std::string>& argv);

  bool RandomizeAllTextInXml(const std::string& path_xml);

 private:
  bool has_xml_changed_ = false;

  std::vector<std::string> lorem_ipsum_ = {
      "lorem", "ipsum", "dolor", "sit", "amet", "consectetur", "adipiscing",
      "elit", "sed", "do", "eiusmod", "tempor", "incididunt", "ut", "labore",
      "et", "dolore", "magna", "aliqua", "ut", "enim", "ad", "minim",
      "veniam,", "quis", "nostrud", "exercitation", "ullamco", "laboris",
      "nisi", "ut", "aliquip", "ex", "ea", "commodo", "consequat", "duis",
      "aute", "irure", "dolor", "in", "reprehenderit", "in", "voluptate",
      "velit", "esse", "cillum", "dolore", "eu", "fugiat", "nulla", "pariatur",
      "excepteur", "sint", "occaecat", "cupidatat", "non", "proident", "sunt",
      "in", "culpa", "qui", "officia", "deserunt", "mollit", "anim", "id",
      "est", "laborum"
  };

  void RandomizeInTextNodes(tinyxml2::XMLElement *node);

  static std::vector<std::string> SplitIntoSpaceSeparatedSegments(
      std::string sentence);

  std::string RandomizeText(std::string str_in);

  std::string GetRandomReplacement(const std::string &segment_in,
                                   const std::string &previous_segment);
};

#endif  // DOCXBOX_DOCX_XML_VISITOR_DOCX_XML_LOREM_H_
