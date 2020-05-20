#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

docxbox=""$BATS_TEST_DIRNAME"/docxbox"
path_docx="test/functional/tmp/cp_mergefields.docx"

base_command="docxbox sfv"
mergefield="MERGEFIELD  Schueler_Anrede"

@test "Output of \"${base_command} {missing argument}\" is an error message" {
  pattern="docxBox Error - Missing argument: Filename of DOCX to be extracted"

  run "${docxbox}" sfv
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]
}

missing_arguments="filename.docx {missing argument}"
@test "Output of \"${base_command} ${missing_arguments}\" is an error message" {
  run "${docxbox}" sfv "${path_docx}"
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: Field identifier" = "${lines[0]}" ]
}

missing_value="filename.docx fieldIdentifier {missing argument}"
@test "Output of \"${base_command} ${missing_value}\" is an error message" {
  run "${docxbox}" sfv "${path_docx}" "${mergefield}"
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: Value to be set" = "${lines[0]}" ]
}

arguments="filename.docx fieldIdentifier fieldValue"
appendix=" the value of the given field is changed"
@test "With \"${base_command} ${arguments}\" ${appendix}" {
  run "${docxbox}" sfv "${path_docx}" "${mergefield}" foobar
  [ "$status" -eq 0 ]

  "${docxbox}" txt "${path_docx}" | grep --count "foobar"
}

@test "Output of \"docxbox sfv nonexistent.docx\" is an error message" {
  err_log="test/functional/tmp/err.log"

  run "${docxbox}" sfv nonexistent.docx
  [ "$status" -ne 0 ]

  "${docxbox}" sfv nonexistent.docx "${mergefield}" foobar 2>&1 | tee "${err_log}"
  cat "${err_log}" | grep --count "docxBox Error - File not found:"
}

title="Output of \"docxbox fieldIdentifier fieldValue wrong_file_type\" "
title+="is an error message"
@test "${title}" {
  pattern="docxBox Error - File is no ZIP archive:"
  err_log="test/functional/tmp/err.log"
  wrong_file_types=(
  "test/functional/tmp/cp_lorem_ipsum.pdf"
  "test/functional/tmp/cp_mock_csv.csv"
  "test/functional/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    "${docxbox}" lorem "${i}" "${mergefield}" foobar 2>&1 | tee "${err_log}"
    cat "${err_log}" | grep --count "${pattern}"
  done
}
