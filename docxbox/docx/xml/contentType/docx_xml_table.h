// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_DOCX_XML_CONTENTTYPE_DOCX_XML_TABLE_H_
#define DOCXBOX_DOCX_XML_CONTENTTYPE_DOCX_XML_TABLE_H_

#include <docxbox/app/app_log.h>
#include <docxbox/docx/xml/docx_xml.h>
#include <docxbox/docx/xml/visitor/docx_xml_remove.h>
#include <docxbox/helper/helper_file.h>
#include <docxbox/helper/helper_string.h>

#include <vendor/tinyxml2/tinyxml2.h>

#include <iostream>
#include <string>
#include <vector>

class docx_xml_table: docx_xml {
 public:
  docx_xml_table(int argc, const std::vector<std::string>& argv);

  bool SetTableValues(const std::string& path_xml,
                      const std::string &specs_json);

  static int GetAmountCellsInFirstRow(const std::string &table_xml);

 private:
  int index_table_ = 0;
  int index_row_start_ = 0;
  int amount_values_to_insert_ = 0;
  std::vector<std::string> values_to_insert_ = {};

  std::string table_xml_;  // xml sub-string: table to be modified

  bool InitFromJson(const std::string &json);

  // Ensure table and row to be updated are contained,
  // extract table markup and replace it by placeholder
  bool PrepareXmlToBeUpdated();

  bool XmlContainsTableToBeUpdated();
  bool TableContainsRowToBeUpdated();

  void ExtractXmlOfTableToBeUpdated();

  bool DuplicateRow(int amount = 1, int index_row = 0);

  // Duplicate row where to start insert texts until enough cells exist
  void EnsureTableHasEnoughCellsToFill();

  void SetValuesInCells();

  static void ClearTextInSiblingCells(tinyxml2::XMLElement *cell);
};

#endif  // DOCXBOX_DOCX_XML_CONTENTTYPE_DOCX_XML_TABLE_H_
