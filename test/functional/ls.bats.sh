#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

DOCXBOX_BINARY="$BATS_TEST_DIRNAME/../tmp/docxbox"

base_command="\"docxbox ls filename.docx"
path_docx="test/tmp/cp_table_unordered_list_images.docx"
path_new_docx="test/tmp/changedFile.docx"

@test "Exit code of ${base_command}\" is zero" {
  run "${DOCXBOX_BINARY}" ls "${path_docx}"
  [ "$status" -eq 0 ]
}

@test "Output of \"docxbox ls {missing argument}\" is an error message" {
  run "${DOCXBOX_BINARY}" ls
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: DOCX filename" = "${lines[0]}" ]
}

@test "Output of ${base_command}\" contains files' and directories' attributes" {
  attributes=(
  "Length"
  "Date"
  "Time"
  "Name")

  for i in "${attributs[@]}"
  do
    "${DOCXBOX_BINARY}" ls "${path_docx}" | grep --count "${i}"
  done
}

@test "Output of ${base_command}\" is contained files" {
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
    "${DOCXBOX_BINARY}" ls "${path_docx}" | grep --count "${i}"
  done
}

@test "Output of ${base_command}\" contains amount of contained files" {
  "${DOCXBOX_BINARY}" ls "${path_docx}" | grep --count '14 files'
}

@test "Output of ${base_command}\" contains files' date and time" {
  "${DOCXBOX_BINARY}" ls "${path_docx}" | grep --count "6/18/2020"
  "${DOCXBOX_BINARY}" ls "${path_docx}" | grep --count "10:30"
}

long_description="contains files with the given file ending"
@test "Output of ${base_command} *.file-ending\" ${long_description}" {
  "${DOCXBOX_BINARY}" ls "${path_docx}" *.jpeg | grep --count "image2.jpeg"
  "${DOCXBOX_BINARY}" ls "${path_docx}" *.xml | grep --count "10 files"
}

@test "With \"${base_command} changedFile.docx\" a side-by-side comparison is displayed" {
  run "${DOCXBOX_BINARY}" lorem "${path_docx}" "${path_new_docx}"

  amount_chars_base=$("${DOCXBOX_BINARY}" ls "${path_docx}" | wc --bytes)
  amount_chars_diff=$("${DOCXBOX_BINARY}" ls "${path_docx}" "${path_new_docx}" | wc --bytes)

  "${DOCXBOX_BINARY}" ls "${path_docx}" "${path_new_docx}" | (( ${amount_chars_base} < ${amount_chars_diff} ))
}

@test "Output of ${base_command} nonexistent.docx\" is an error message" {
  err_log="test/tmp/err.log"

  run "$BATS_TEST_DIRNAME"/docxbox ls nonexistent.docx
  [ "$status" -ne 0 ]

  "$BATS_TEST_DIRNAME"/docxbox ls nonexistent.docx 2>&1 | tee "${err_log}"
  cat "${err_log}" | grep --count "docxBox Error - File not found:"
}

@test "Output of ${base_command} wrong_file_type\" is an error message" {
  err_log="test/tmp/err.log"
  wrong_file_types=(
  "test/tmp/cp_lorem_ipsum.pdf"
  "test/tmp/cp_mock_csv.csv"
  "test/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    "$BATS_TEST_DIRNAME"/docxbox ls "${i}" 2>&1 | tee "${err_log}"
    cat "${err_log}" | grep --count "docxBox Error - File is no ZIP archive:"
  done
}
