#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

DOCXBOX_BINARY="$BATS_TEST_DIRNAME/../tmp/docxbox"
path_docx="test/tmp/cp_table_unordered_list_images.docx"

base_command="docxbox lsij filename.docx"
description="are contained images as JSON"

@test "Exit code of \"${base_command}\" is zero" {
  run "${DOCXBOX_BINARY}" lsij "${path_docx}"
  [ "$status" -eq 0 ]
}

@test "Output of \"docxbox lsij {missing argument}\" is an error message" {
  run "${DOCXBOX_BINARY}" lsij
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: DOCX filename" = "${lines[0]}" ]
}

@test "Output of \"${base_command}\" is contained images as JSON" {
  "${DOCXBOX_BINARY}" lsij "${path_docx}" | grep --count ""image2.jpeg""
}

@test "Output of \"docxbox lsi filename.docx --json\" ${description}" {
  "${DOCXBOX_BINARY}" lsi "${path_docx}" --json | grep --count ""image2.jpeg""
}

@test "Output of \"docxbox lsi filename.docx -j\" ${description}" {
  "${DOCXBOX_BINARY}" lsi "${path_docx}" -j | grep --count ""image2.jpeg""
}

@test "Output of \"docxbox ls filename.docx -ij\" ${description}" {
  "${DOCXBOX_BINARY}" lsi "${path_docx}" -ij | grep --count ""image2.jpeg""
}

@test "Output of \"docxbox ls filename.docx --images --json\" ${description}" {
  "${DOCXBOX_BINARY}" ls "${path_docx}" --images --json | grep --count ""image2.jpeg""
}

@test "Output of \"docxbox lsij nonexistent.docx\" is an error message" {
  err_log="test/tmp/err.log"

  run "$BATS_TEST_DIRNAME"/docxbox lsij nonexistent.docx
  [ "$status" -ne 0 ]

  "${DOCXBOX_BINARY}" lsij nonexistent.docx 2>&1 | tee "${err_log}"
  cat "${err_log}" | grep --count "docxBox Error - File not found:"
}

@test "Output of \"docxbox lsij wrong_file_type\" is an error message" {
  err_log="test/tmp/err.log"
  wrong_file_types=(
  "test/tmp/cp_lorem_ipsum.pdf"
  "test/tmp/cp_mock_csv.csv"
  "test/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    "${DOCXBOX_BINARY}" lsij "${i}" 2>&1 | tee "${err_log}"
    cat "${err_log}" | grep --count "docxBox Error - File is no ZIP archive:"
  done
}
