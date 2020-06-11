#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

base_command="\"docxbox lsd"

docxbox=""$BATS_TEST_DIRNAME"/docxbox"
path_docx="test/functional/tmp/cp_mergefields.docx"

merge_field="MERGEFIELD"
merge_format="\* MERGEFORMAT"

@test "Exit code of ${base_command} filename.docx\" is zero" {
  run "$BATS_TEST_DIRNAME"/docxbox lsd "${path_docx}"
  [ "$status" -eq 0 ]
}

@test "Output of ${base_command} {missing argument}\" is an error message" {
  run "$BATS_TEST_DIRNAME"/docxbox lsd
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: Filename of DOCX to be extracted" = "${lines[0]}" ]
}

@test "With ${base_command} filename.docx\" the fields in the docx are listed" {
  "${docxbox}" lsd "${path_docx}" | grep --count "${merge_field}"
  "${docxbox}" lsd "${path_docx}" | grep --count "${merge_format}"
}

@test "With ${base_command} filename.docx\" the fields in the header are listed" {
  "${docxbox}" lsd "${path_docx}" | grep --count "MERGEFIELD  Mergefield_Header"
}

@test "With ${base_command} filename.docx\" the fields in the footer are listed" {
  "${docxbox}" lsd "${path_docx}" | grep --count "MERGEFIELD  Mergefield_Footer"
}

title="With \"docxbox ls filename.docx --fields\" "
title+="the fields in the docx are listed"
@test "$title" {
  "${docxbox}" lsd "${path_docx}" | grep --count "${merge_field}"
  "${docxbox}" lsd "${path_docx}" | grep --count "${merge_format}"
}

@test "With \"docxbox ls filename.docx -d\" the fields in the docx are listed" {
  "${docxbox}" ls "${path_docx}" -d | grep --count "${merge_field}"
  "${docxbox}" ls "${path_docx}" -d | grep --count "${merge_format}"
}

@test "Output of \"docxbox lsd nonexistent.docx\" is an error message" {
  err_log="test/functional/tmp/err.log"

  run "$BATS_TEST_DIRNAME"/docxbox lsd nonexistent.docx
  [ "$status" -ne 0 ]

  "$BATS_TEST_DIRNAME"/docxbox lsd nonexistent.docx 2>&1 | tee "${err_log}"
  cat "${err_log}" | grep --count "docxBox Error - File not found:"
}

@test "Output of ${base_command} wrong_file_type\" is an error message" {
  err_log="test/functional/tmp/err.log"
  wrong_file_types=(
  "test/functional/tmp/cp_lorem_ipsum.pdf"
  "test/functional/tmp/cp_mock_csv.csv"
  "test/functional/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    "$BATS_TEST_DIRNAME"/docxbox lsd "${i}" 2>&1 | tee "${err_log}"
    cat "${err_log}" | grep --count "docxBox Error - File is no ZIP archive:"
  done
}
