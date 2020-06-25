#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

DOCXBOX_BINARY="$BATS_TEST_DIRNAME/../tmp/docxbox"

path_docx="test/tmp/cp_table_unordered_list_images.docx"

@test "Output of \"docxbox rmt {missing filename}\" is an error message" {
  run "${DOCXBOX_BINARY}" rmt
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: DOCX filename" = "${lines[0]}" ]
}

title="Output of \"docxbox rmt filename.docx {missing arguments}\" "
title+="is an error message"
@test "${title}" {
  pattern="docxBox Error - Missing argument: \
String left-hand-side of part to be removed"

  run "${DOCXBOX_BINARY}" rmt "${path_docx}"
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]
}

@test "Output of \"docxbox rmt filename.docx leftHandString {missing_argument}\" is an error message" {
  pattern="docxBox Error - Missing argument: \
String right-hand-side of part to be removed"

  run "${DOCXBOX_BINARY}" rmt "${path_docx}" "FooBar"
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]
}

@test "With \"docxbox rmt filename.docx leftHandString rightHandString\" removes text between and including given strings" {
  pattern="Fugiat excepteursed in qui sit velit duis veniam."

  "${DOCXBOX_BINARY}" lsl "${path_docx}" ${pattern} | grep --count "word/document.xml"

  run "${DOCXBOX_BINARY}" rmt "${path_docx}" "Fugiat" "."
  [ "$status" -eq 0 ]

  "${DOCXBOX_BINARY}" txt "${path_docx}" | grep --count --invert-match "${pattern}"
}

@test "Output of \"docxbox rmt nonexistent.docx\" is an error message" {
  err_log="test/tmp/err.log"

  run "${DOCXBOX_BINARY}" rmt nonexistent.docx
  [ "$status" -ne 0 ]

  "${DOCXBOX_BINARY}" rmt nonexistent.docx Dolore incididunt 2>&1 | tee "${err_log}"
  cat "${err_log}" | grep --count "docxBox Error - File not found:"
}

@test "Output of \"docxbox rmt wrong_file_type\" is an error message" {
  pattern="docxBox Error - File is no ZIP archive:"
  err_log="test/tmp/err.log"
  wrong_file_types=(
  "test/tmp/cp_lorem_ipsum.pdf"
  "test/tmp/cp_mock_csv.csv"
  "test/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    "${DOCXBOX_BINARY}" rmt "${i}" Dolore incididunt 2>&1 | tee "${err_log}"
    cat "${err_log}" | grep --count "${pattern}"
  done
}
