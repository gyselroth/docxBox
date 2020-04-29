// Copyright (c) 2020 gyselroth GmbH

#ifndef DOCXBOX_DOCX_DOCX_ARCHIVE_LIST_H_
#define DOCXBOX_DOCX_DOCX_ARCHIVE_LIST_H_

#include <docxbox/app/app_argument.h>
#include <docxbox/docx/docx_archive.h>
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

class docx_archive_list:docx_archive {
 public:
  docx_archive_list(int argc, char **argv);

  bool LocateFilesContainingString(bool as_json = false);

  // List files inside DOCX archive and their attributes
  bool ListFiles(bool as_json, bool images_only = false);
  bool ListImages(bool as_json);

  // List data stored within XML(s) of DOCX
  bool ListFonts(bool as_json);
  bool ListFields(bool as_json);
  bool ListMeta(bool as_json);

 private:
  void InitLocateFilesContaining(bool &as_json, std::string &needle) const;
};

#endif  // DOCXBOX_DOCX_DOCX_ARCHIVE_LIST_H_
