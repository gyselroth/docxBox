// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_DOCX_DOCX_ARCHIVE_LIST_H_
#define DOCXBOX_DOCX_DOCX_ARCHIVE_LIST_H_

#include <docxbox/app/app_argument.h>
#include <docxbox/docx/docx_archive.h>
#include <docxbox/docx/docx_fileListCompare.h>
#include <docxbox/docx/docx_fontTable.h>
#include <docxbox/docx/docx_meta.h>
#include <docxbox/docx/renderer/docx_renderer_table.h>
#include <docxbox/docx/xml/docx_xml_field.h>
#include <docxbox/docx/xml/docx_xml_lorem.h>
#include <docxbox/docx/xml/docx_xml_remove.h>
#include <docxbox/docx/xml/docx_xml_replace.h>
#include <docxbox/docx/xml/docx_xml_to_plaintext.h>
#include <docxbox/docx/xml/docx_xml.h>
#include <docxbox/helper/helper_cli.h>
#include <docxbox/helper/helper_dateTime.h>
#include <docxbox/helper/helper_file.h>
#include <docxbox/helper/helper_string.h>

#include <iostream>
#include <vector>
#include <memory>
#include <string>

class docx_archive_list : public docx_archive {
 public:
  docx_archive_list(int argc, char **argv);

  bool LocateFilesContainingString(bool as_json = false);

  // List files inside DOCX archive and their attributes
  bool ListFilesInDocx(bool as_json, bool images_only = false);
  bool ListImageFilesInDocx(bool as_json);

  // List data stored within XML(s) of DOCX
  bool ListReferencedFonts(bool as_json);
  bool ListFieldsFromXmls(bool as_json);
  bool ListMetaFromXmls(bool as_json);

 private:
  void InitLocateFilesContaining(bool &as_json, std::string &needle) const;

  // Output two DOCX file lists side-by-side
  void ListFilesInDocxCompare(bool as_json,
                              bool images_only,
                              const std::string &file_ending,
                              std::string &file_list_1,
                              std::string &summary_1);
};

#endif  // DOCXBOX_DOCX_DOCX_ARCHIVE_LIST_H_
