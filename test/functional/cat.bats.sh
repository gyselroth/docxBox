#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

DOCXBOX_BINARY="$BATS_TEST_DIRNAME/docxbox"

path_docx="test/tmp/cp_table_unordered_list_images.docx"
path_xml="word/document.xml"

@test "Output of \"docxbox cat {missing argument}\" is an error message" {
  run "${DOCXBOX_BINARY}" cat
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: DOCX filename" = "${lines[0]}" ]
}

@test "Output of \"docxbox cat filename.docx {missing argument}\" is an error message" {
  run "${DOCXBOX_BINARY}" cat "${path_docx}"
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: File to be displayed" = "${lines[0]}" ]
}

@test "With \"docxbox cat filename.docx filename.xml\" the XML is displayed" {
  "${DOCXBOX_BINARY}" cat "${path_docx}" "${path_xml}" | grep "^[[:space:]]\{4\}"
}
