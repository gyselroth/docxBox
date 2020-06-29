// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_DOCX_ARCHIVE_DOCX_ARCHIVE_REPLACE_H_
#define DOCXBOX_DOCX_ARCHIVE_DOCX_ARCHIVE_REPLACE_H_

#include <docxbox/app/app_argument.h>
#include <docxbox/docx/archive/docx_archive.h>
#include <docxbox/docx/component/contentTypes.h>
#include <docxbox/docx/component/fontTable.h>
#include <docxbox/docx/component/meta.h>
#include <docxbox/docx/renderer/contentType/docx_renderer_table.h>
#include <docxbox/docx/xml/contentType/docx_xml_field.h>
#include <docxbox/docx/xml/visitor/docx_xml_preprocess.h>
#include <docxbox/docx/xml/visitor/docx_xml_lorem.h>
#include <docxbox/docx/xml/visitor/docx_xml_remove.h>
#include <docxbox/docx/xml/visitor/docx_xml_replace.h>
#include <docxbox/docx/xml/visitor/docx_xml_to_plaintext.h>
#include <docxbox/docx/xml/docx_xml.h>
#include <docxbox/helper/helper_cli.h>
#include <docxbox/helper/helper_dateTime.h>
#include <docxbox/helper/helper_file.h>
#include <docxbox/helper/helper_string.h>

#include <iostream>
#include <vector>
#include <memory>
#include <string>

class docx_archive_replace : public docx_archive {
 public:
  docx_archive_replace(
      int argc, const std::vector<std::string>& argv, bool is_batch_mode);

  bool ReplaceImage();

  bool ReplaceText();

  bool RemoveBetweenText();

  bool ReplaceAllTextByLoremIpsum();

  bool SetFieldValue();

 private:
  void InitPathDocxOutForReplaceText(std::string *path_docx_out,
                                     bool *overwrite_source_docx) const;

  // New image file added, requires adding resp. relation
  bool added_image_file_ = false;
};

#endif  // DOCXBOX_DOCX_ARCHIVE_DOCX_ARCHIVE_REPLACE_H_
