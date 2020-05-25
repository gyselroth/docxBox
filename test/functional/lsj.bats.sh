#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

docxbox=""$BATS_TEST_DIRNAME"/docxbox"

path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

longhand_command="docxbox ls filename.docx"
description="contains files' and directories' attributes"

attributes=(
  "length"
  "date"
  "time"
  "file")

@test "Output of \"docxbox lsj filename.docx\" ${description}" {
  for i in "${attributes[@]}"
  do
    "${docxbox}" lsj "${path_docx}" | grep --count "${i}"
  done
}

@test "Output of \"${longhand_command} --json\" ${description}" {
  for i in "${attributes[@]}"
  do
    "${docxbox}" ls "${path_docx}" --json | grep --count "${i}"
  done
}

@test "Output of \"${longhand_command} -j\" ${description}" {
  for i in "${attributes[@]}"
  do
    "${docxbox}" ls "${path_docx}" -j | grep --count "${i}"
  done
}

@test "Output of \"docxbox lsj filename.docx\" are contained files" {
search_values=(
"[Content_Types].xml"
"_rels/.rels"
"word/_rels/document.xml.rels"
"word/document.xml"
"word/theme/theme1.xml"
"word/media/image1.png"
"word/media/image2.jpeg"
"word/settings.xml"
"word/webSettings.xml"
"word/styles.xml"
"word/numbering.xml"
"docProps/core.xml"
"word/fontTable.xml"
"docProps/app.xml")

  for i in "${search_values[@]}"
  do
    "${docxbox}" lsj "${path_docx}" | grep --count "${i}"
  done
}

@test "Output of \"docxbox lsj filename.docx\" contains files' date and time" {
  "${docxbox}" lsj "${path_docx}" | grep --count "1/1/1980"
  "${docxbox}" lsj "${path_docx}" | grep --count "0:0"
}

@test "Output of \"docxbox lsj {missing argument}\" is an error message" {
  run "${docxbox}" lsj
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: DOCX filename" = "${lines[0]}" ]
}

@test "Output of \"docxbox lsj nonexistent.docx\" is an error message" {
  err_log="test/functional/tmp/err.log"

  run "$BATS_TEST_DIRNAME"/docxbox lsj nonexistent.docx
  [ "$status" -ne 0 ]

  "$BATS_TEST_DIRNAME"/docxbox lsj nonexistent.docx 2>&1 | tee "${err_log}"
  cat "${err_log}" | grep --count "docxBox Error - File not found:"
}

@test "Output of \"docxbox lsj wrong_file_type\" is an error message" {
  err_log="test/functional/tmp/err.log"
  wrong_file_types=(
  "test/functional/tmp/cp_lorem_ipsum.pdf"
  "test/functional/tmp/cp_mock_csv.csv"
  "test/functional/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    "$BATS_TEST_DIRNAME"/docxbox lsj "${i}" 2>&1 | tee "${err_log}"
    cat "${err_log}" | grep --count "docxBox Error - File is no ZIP archive:"
  done
}
