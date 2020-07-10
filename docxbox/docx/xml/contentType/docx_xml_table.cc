// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/docx/xml/contentType/docx_xml_table.h>
#include <vendor/json/single_include/nlohmann/json.hpp>

docx_xml_table::docx_xml_table(
    int argc,
    const std::vector<std::string> &argv) : docx_xml(argc, argv) {
}

bool docx_xml_table::SetTableValues(const std::string &path_xml,
                                    const std::string &specs_json) {
  if (!InitFromJson(specs_json)) return false;

  path_xml_file_ = path_xml;
  helper::File::GetFileContents(path_xml, &xml_);

  // If given markup allows setting table values:
  // Extract table markup from xml_ into table_xml_
  if (!PrepareXmlToBeUpdated()) return false;

  // Modify markup in table_xml_
  EnsureTableHasEnoughCellsToFill();
  SetValuesInCells();

  // Replace table markup in xml_ than set initialize doc_ from
  ReInsertTableIntoXml();
  SetDocFromXml();

  if (doc_.ErrorID() != 0) return false;

  return tinyxml2::XML_SUCCESS != doc_.SaveFile(path_xml.c_str(), true)
         ? docxbox::AppLog::NotifyError("Failed saving: " + path_xml)
         : true;
}

bool docx_xml_table::InitFromJson(const std::string &json) {
  try {
    auto json_inner = nlohmann::json::parse(json);

    for (nlohmann::json::iterator it = json_inner.begin();
         it != json_inner.end();
         ++it) {
      try {
        const std::string &key = it.key();

        if (key == "table") {
          index_table_ = it.value();
// TODO(kay): wrap into try/catch and allow being given as string also
//          index_table_ = std::stoi(std::string(it.value()));
        } else if (key == "row") {
          index_row_start_ = it.value();
// TODO(kay): wrap into try/catch and allow being given as string also
//          std::stoi(std::string(value));
//          index_row_start_ = std::stoi(std::string(value));
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
      } catch (nlohmann::json::invalid_iterator &e) {
        continue;
      }
    }

    return index_table_ == 0
               || index_row_start_ == 0
               || amount_values_to_insert_ == 0
               || values_to_insert_.empty()
           ? docxbox::AppLog::NotifyError("Invalid table config")
           : true;
  } catch (nlohmann::detail::parse_error &e) {
    return docxbox::AppLog::NotifyError("Parse error - Invalid JSON: " + json);
  }
}

// If table and row to be updated are contained: extract table markup
bool docx_xml_table::PrepareXmlToBeUpdated() {
  if (!XmlContainsTableToBeUpdated()) return docxbox::AppLog::NotifyError(
        std::string("Invalid document- must contain at least")
            + std::to_string(index_table_) + " tables");

  ExtractTableMarkup();

  if (!TableContainsRowToBeUpdated()) return docxbox::AppLog::NotifyError(
        std::string("Invalid document- must contain at least")
            + std::to_string(index_row_start_) + " rows "
                                                 "in table "
            + std::to_string(index_table_));

  return true;
}

void docx_xml_table::ExtractTableMarkup() {
  offset_table_start_ =
      helper::String::FindNthOccurrence(xml_, "<w:tbl", index_table_);

  offset_table_end_ = xml_.find("</w:tbl>", offset_table_start_);

  auto len = offset_table_end_ - offset_table_start_ + 8;
  table_xml_ = xml_.substr(offset_table_start_, len);
}

void docx_xml_table::ReInsertTableIntoXml() {
  xml_.replace(
      offset_table_start_,
      offset_table_end_ - offset_table_start_ + 8,
      table_xml_);
}

bool docx_xml_table::TableContainsRowToBeUpdated() {
  return helper::String::SubStrCount(table_xml_, "<w:tr") >= index_row_start_;
}

bool docx_xml_table::XmlContainsTableToBeUpdated() {
  return helper::String::SubStrCount(xml_, "<w:tbl") >= index_table_;
}

// Duplicate row where to start insert texts until enough cells exist
void docx_xml_table::EnsureTableHasEnoughCellsToFill() {
  int amount_cells_per_row = GetAmountCellsInFirstRow(table_xml_);

  if (amount_cells_per_row < amount_values_to_insert_) {
    int amount_rows_missing = 0;
    int amount_cells_given = amount_cells_per_row;

    while (amount_cells_given < amount_values_to_insert_) {
      ++amount_rows_missing;
      amount_cells_given += amount_cells_per_row;
    }

    DuplicateRow(index_row_start_, amount_rows_missing);
  }
}

int docx_xml_table::GetAmountCellsInFirstRow(const std::string &table_xml) {
  tinyxml2::XMLDocument table;
  table.Parse(table_xml.c_str());

  tinyxml2::XMLElement *row =
      table.FirstChildElement("w:tbl")->FirstChildElement("w:tr");

  auto cell = row->FirstChildElement("w:tc");

  if (nullptr == cell) return 0;

  int amount = 1;

  do {
    cell = cell->NextSiblingElement();

    if (cell == nullptr) break;

    ++amount;
  } while (true);

  return amount;
}

bool docx_xml_table::DuplicateRow(int amount, int index_row) {
  auto offset_start = index_row == 0
      ? table_xml_.find("<w:tr")
      : helper::String::FindNthOccurrence(table_xml_, "<w:tr", index_row);

  if (-1 == offset_start) return false;

  int offset_end = table_xml_.find("</w:tr>");

  auto len = offset_end - offset_start + 7;
  auto row = table_xml_.substr(offset_start, len);
  auto duplication = row;

  for (int i=0; i < index_row; i++)
    duplication += row;

  table_xml_.replace(offset_start, len, duplication);

  return true;
}

void docx_xml_table::SetValuesInCells() {
  tinyxml2::XMLDocument table;
  table.Parse(table_xml_.c_str());

  tinyxml2::XMLElement *row =
      table.FirstChildElement("w:tbl")->FirstChildElement("w:tr");

  if (index_row_start_ > 1) {
    for (int index_row = 1; index_row  < index_row_start_; index_row++)
      row = row->NextSiblingElement();
  }

  int index_cell = 0;
  auto cell = row->FirstChildElement("w:tc");

  tinyxml2::XMLElement *wt;

  do {
    // Set text in cell
    auto para = cell->FirstChildElement("w:p");
    auto run = para->FirstChildElement("w:r");
    wt = run->FirstChildElement("w:t");

    // TODO(kay): if wt == nullptr (if possible?) - insert one

    wt->SetText(values_to_insert_[index_cell].c_str());

    // TODO(kay): remove all sibling w:t's, w:r's, w:p's
//    tinyxml2::XMLElement *sibling;
//
//    do {
//      sibling = wt->NextSiblingElement();
//      sibling->Parent()->DeleteChild(sibling);
//    } while (sibling != nullptr);

    ++index_cell;

    if (index_cell >= amount_values_to_insert_) break;

    cell = cell->NextSiblingElement();

    if (nullptr == cell) {
      // Advance to 1st cell of next row
      row = row->NextSiblingElement();
      if (nullptr == row) break;

      cell = row->FirstChildElement("w:tc");
      if (nullptr == cell) break;
    }
  } while (index_cell < amount_values_to_insert_);

  ClearTextInSiblingCells(cell);

  tinyxml2::XMLPrinter printer;
  table.Print(&printer);
  table_xml_ = printer.CStr();
}

void docx_xml_table::ClearTextInSiblingCells(tinyxml2::XMLElement *cell) {
  do {
    cell = cell->NextSiblingElement();

    if (nullptr == cell) break;

    auto para = cell->FirstChildElement("w:p");
    auto run = para->FirstChildElement("w:r");
    auto wt = run->FirstChildElement("w:t");

    wt->SetText("");
  } while (true);
}
