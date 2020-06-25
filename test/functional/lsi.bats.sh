#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

DOCXBOX_BINARY="$BATS_TEST_DIRNAME/../tmp/docxbox"
path_docx="test/tmp/cp_table_unordered_list_images.docx"

base_command="docxbox lsi filename.docx"

@test "Exit code of \"${base_command}\" is zero" {
  run "${DOCXBOX_BINARY}" lsi "${path_docx}"
  [ "$status" -eq 0 ]
}

@test "Output of \"docxbox lsi {missing argument}\" is an error message" {
  pattern="docxBox Error - Missing argument: Filename of DOCX to be extracted"

  run "${DOCXBOX_BINARY}" lsi
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]
}

@test "Output of \"${base_command}\" contains files' and directories' attributes" {
  attributes=(
  "Length"
  "Date"
  "Time"
  "Name")

  for i in "${attributes[@]}"
  do
    "${DOCXBOX_BINARY}" ls "${path_docx}" | grep --count "${i}"
  done
}

@test "Output of \"${base_command}\" is contained images" {
  run "${DOCXBOX_BINARY}" lsi "${path_docx}"
  [ "$status" -eq 0 ]
  "${DOCXBOX_BINARY}" lsi "${path_docx}" | grep --count "image2.jpeg"
}

@test "Output of \"docxbox ls filename.docx -i\" is contained images" {
  "${DOCXBOX_BINARY}" ls "${path_docx}" -i | grep --count "image2.jpeg"
}

@test "Output of \"docxbox ls filename.docx --images\" is contained images" {
  "${DOCXBOX_BINARY}" ls "${path_docx}" --images | grep --count "image2.jpeg"
}

@test "Output of \"docxbox lsi nonexistent.docx\" is an error message" {
  err_log="test/tmp/err.log"

  run "$BATS_TEST_DIRNAME"/docxbox lsi nonexistent.docx
  [ "$status" -ne 0 ]

  "${DOCXBOX_BINARY}" lsi nonexistent.docx 2>&1 | tee "${err_log}"
  cat "${err_log}" | grep --count "docxBox Error - File not found:"
}

@test "Output of \"docxbox lsi wrong_file_type\" is an error message" {
  err_log="test/tmp/err.log"
  wrong_file_types=(
  "test/tmp/cp_lorem_ipsum.pdf"
  "test/tmp/cp_mock_csv.csv"
  "test/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    "${DOCXBOX_BINARY}" lsi "${i}" 2>&1 | tee "${err_log}"
    cat "${err_log}" | grep --count "docxBox Error - File is no ZIP archive:"
  done
}
