#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

VALGRIND="valgrind -v --leak-check=full\
 --log-file=test/assets/documents/other/mem-leak.log"

if $IS_VALGRIND_TEST;
then
  DOCXBOX_BINARY="${VALGRIND} $BATS_TEST_DIRNAME/../tmp/docxbox"
else
  DOCXBOX_BINARY="$BATS_TEST_DIRNAME/../tmp/docxbox"
fi

path_docx="test/tmp/cp_table_unordered_list_images.docx"

base_command="docxbox lsm filename.docx"

@test "Exit code of \"${base_command}\" is zero" {
  run ${DOCXBOX_BINARY} lsm "${path_docx}"
  [ "$status" -eq 0 ]
}

@test "Output of \"docxbox lsm {missing argument}\" is an error message" {
  run ${DOCXBOX_BINARY} lsm
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: Filename of DOCX to be extracted" = "${lines[0]}" ]
}

@test "Output of \"${base_command}\" contains information about the xml schema" {
  xml_schema="xmlSchema: http://schemas.openxmlformats.org/officeDocument/2006"

  ${DOCXBOX_BINARY} lsm "${path_docx}" | grep --count "${xml_schema}"
}

title="Output of \"${base_command}\" "
title+="contains information about the creation time and date"
@test "${title}" {
  created="created: 2020-06-18T10:30:11Z"

  ${DOCXBOX_BINARY} lsm "${path_docx}" | grep --count "${created}"
}

@test "Output of \"${base_command}\" contains language information" {
  ${DOCXBOX_BINARY} lsm "${path_docx}" | grep --count "language: en-US"
}

@test "Output of \"${base_command}\" contains information about the revision" {
  ${DOCXBOX_BINARY} lsm "${path_docx}" | grep --count "revision: 2"
}

@test "Output of \"docxbox lsm nonexistent.docx\" is an error message" {
  err_log="test/tmp/err.log"

  run ${DOCXBOX_BINARY} lsm nonexistent.docx
  [ "$status" -ne 0 ]

  ${DOCXBOX_BINARY} lsm nonexistent.docx 2>&1 | tee "${err_log}"
  cat "${err_log}" | grep --count "docxBox Error - File not found:"
}

@test "Output of \"docxbox lsm wrong_file_type\" is an error message" {
  pattern="docxBox Error - File is no ZIP archive:"
  err_log="test/tmp/err.log"
  wrong_file_types=(
  "test/tmp/cp_lorem_ipsum.pdf"
  "test/tmp/cp_mock_csv.csv"
  "test/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    ${DOCXBOX_BINARY} lsm "${i}" 2>&1 | tee "${err_log}"
    cat "${err_log}" | grep --count "${pattern}"
  done
}
