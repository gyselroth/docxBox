// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/docx/xml/contentType/docx_xml_table.h>
#include <vendor/json/single_include/nlohmann/json.hpp>

docx_xml_table::docx_xml_table(
    int argc,
    const std::vector<std::string> &argv) : docx_xml(argc, argv) {
}

bool docx_xml_table::SetTableValues(const std::string& path_xml,
                                  const std::string &specs_json) {
  if (!InitFromJson(specs_json)) return false;

  path_xml_file_ = path_xml;
  helper::File::GetFileContents(path_xml, &xml_);

  if (!XmlContainsValidTableForSetTableValues()) return false;

//  PrepareTableCells();  TODO(kay): ensure enough rows and cells are given
//  FillCellsWithValues();  TODO(kay): implement

  SetDocFromXml();

  if (doc_.ErrorID() != 0) return false;

  return tinyxml2::XML_SUCCESS != doc_.SaveFile(path_xml.c_str(), true)
         ? docxbox::AppLog::NotifyError("Failed saving: " + path_xml)
         : true;
}

bool docx_xml_table::InitFromJson(const std::string &json) {
  auto json_outer = nlohmann::json::parse(json);

  for (auto& json_inner : json_outer) {
    for (nlohmann::json::iterator it = json_inner.begin();
         it != json_inner.end();
         ++it) {
      const std::string& key = it.key();

      if (key == "table") {
        index_table_ = std::stoi(std::string(it.value()));
      } else if (key == "row") {
        index_row_start_ = std::stoi(std::string(it.value()));
      } else if (key == "values" || key == "data") {
        amount_values_to_insert_ = 0;
        values_to_insert_.clear();

        auto values = it.value();

        for (nlohmann::json::iterator it_header = values.begin();
             it_header != values.end();
             ++it_header) {
          values_to_insert_.push_back(it_header.value());

          ++amount_values_to_insert_;
        }
      } else if (key == "pre" || key == "post") {
        // TODO(kay): implement
//        ExtractPreOrPostfix(it);
      }
    }
  }

  return index_table_ == 0
             || index_row_start_ == 0
             || amount_values_to_insert_ == 0
             || values_to_insert_.empty()
         ? docxbox::AppLog::NotifyError("Invalid table config")
         : true;
}

bool docx_xml_table::XmlContainsValidTableForSetTableValues() {
  if (helper::String::SubStrCount(xml_, "<w:tbl") < index_table_)
    return docxbox::AppLog::NotifyError(
        std::string("Invalid document- must contain at least")
            + std::to_string(index_table_) + " tables");

  int offset_table =
      helper::String::FindNthOccurrence(xml_, "<w:tbl", index_table_);

  if (helper::String::SubStrCount(xml_, "<w:tr", offset_table) < index_table_)
    return docxbox::AppLog::NotifyError(
        std::string("Invalid document- must contain at least")
            + std::to_string(index_row_start_) + " rows "
        "in table " + std::to_string(index_table_));

  return false;
}
