#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

DOCXBOX_BINARY="$BATS_TEST_DIRNAME/../tmp/docxbox"
path_docx="test/tmp/cp_mergefields.docx"

base_command="docxbox sfv"

mergefield="MERGEFIELD  Mergefield_One"
mergefield_header="MERGEFIELD  Mergefield_Header"
mergefield_footer="MERGEFIELD  Mergefield_Footer"

@test "Output of \"${base_command} {missing argument}\" is an error message" {
  pattern="docxBox Error - Missing argument: Filename of DOCX to be extracted"

  run "${DOCXBOX_BINARY}" sfv
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]
}

missing_arguments="filename.docx {missing argument}"
@test "Output of \"${base_command} ${missing_arguments}\" is an error message" {
  run "${DOCXBOX_BINARY}" sfv "${path_docx}"
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: Field identifier" = "${lines[1]}" ]
}

missing_value="filename.docx fieldIdentifier {missing argument}"
@test "Output of \"${base_command} ${missing_value}\" is an error message" {
  run "${DOCXBOX_BINARY}" sfv "${path_docx}" "${mergefield}"
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: Value to be set" = "${lines[1]}" ]
}

arguments="filename.docx fieldIdentifier fieldValue"
appendix=" the value of the given field is changed"
@test "With \"${base_command} ${arguments}\" ${appendix}" {
  run "${DOCXBOX_BINARY}" sfv "${path_docx}" "${mergefield}" foobar
  [ "$status" -eq 0 ]

  "${DOCXBOX_BINARY}" txt "${path_docx}" | grep --count "foobar"
}

@test "With \"${base_command} ${arguments}\" the value of the mergefield in the header gets changed" {
  run "${DOCXBOX_BINARY}" sfv "${path_docx}" "${mergefield_header}" foobar
  [ "$status" -eq 0 ]

  "${DOCXBOX_BINARY}" txt "${path_docx}" | grep --count "foobar"
}

@test "With \"${base_command} ${arguments}\" the value of the mergefield in the footer gets changed" {
  run "${DOCXBOX_BINARY}" sfv "${path_docx}" "${mergefield_footer}" foobar
  [ "$status" -eq 0 ]

  "${DOCXBOX_BINARY}" txt "${path_docx}" | grep --count "foobar"
}

@test "Output of \"docxbox sfv nonexistent.docx\" is an error message" {
  err_log="test/tmp/err.log"

  run "${DOCXBOX_BINARY}" sfv nonexistent.docx
  [ "$status" -ne 0 ]

  "${DOCXBOX_BINARY}" sfv nonexistent.docx "${mergefield}" foobar 2>&1 | tee "${err_log}"
  cat "${err_log}" | grep --count "docxBox Error - File not found:"
}

title="Output of \"docxbox fieldIdentifier fieldValue wrong_file_type\" "
title+="is an error message"
@test "${title}" {
  pattern="docxBox Error - File is no ZIP archive:"
  err_log="test/tmp/err.log"
  wrong_file_types=(
  "test/tmp/cp_lorem_ipsum.pdf"
  "test/tmp/cp_mock_csv.csv"
  "test/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    "${DOCXBOX_BINARY}" lorem "${i}" "${mergefield}" foobar 2>&1 | tee "${err_log}"
    cat "${err_log}" | grep --count "${pattern}"
  done
}
