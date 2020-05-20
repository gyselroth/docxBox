#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

docxbox=""$BATS_TEST_DIRNAME"/docxbox"
path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

base_command="docxbox lsi filename.docx"

@test "Exit code of \"${base_command}\" is zero" {
  run "${docxbox}" lsi "${path_docx}"
  [ "$status" -eq 0 ]
}

@test "Output of \"docxbox lsi {missing argument}\" is an error message" {
  pattern="docxBox Error - Missing argument: Filename of DOCX to be extracted"

  run "${docxbox}" lsi
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
    "${docxbox}" ls "${path_docx}" | grep --count "${i}"
  done
}

@test "Output of \"${base_command}\" is contained images" {
  run "${docxbox}" lsi "${path_docx}"
  [ "$status" -eq 0 ]
  "${docxbox}" lsi "${path_docx}" | grep --count image1.jpeg
}

@test "Output of \"docxbox ls filename.docx -i\" is contained images" {
  "${docxbox}" ls "${path_docx}" -i | grep --count image1.jpeg
}

@test "Output of \"docxbox ls filename.docx --images\" is contained images" {
  "${docxbox}" ls "${path_docx}" --images | grep --count image1.jpeg
}

@test "Output of \"docxbox lsi wrong_file_type\" is an error message" {
  err_log="test/functional/tmp/err.log"
  wrong_file_types=(
  "test/functional/tmp/cp_lorem_ipsum.pdf"
  "test/functional/tmp/cp_mock_csv.csv"
  "test/functional/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    "${docxbox}" lsi "${i}" 2>&1 | tee "${err_log}"
    cat "${err_log}" | grep --count "docxBox Error - File is no ZIP archive:"
  done
}
