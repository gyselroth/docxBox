#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

docxbox=""$BATS_TEST_DIRNAME"/docxbox"
base_command="docxbox lorem filename.docx"

@test "Exit code of \"${base_command}\" is zero" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  run "${docxbox}" lorem "${path_docx}"
  [ "$status" -eq 0 ]
}

@test "Output of \"docxbox lorem {missing argument}\" is an error message" {
  pattern="docxBox Error - Missing argument: Filename of DOCX to be extracted"

  run "${docxbox}" lorem
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]
}

@test "With \"${base_command}\" text gets replaced by dummy text" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"
  pattern="Culpa ad eiusmod"

  run "${docxbox}" lorem "${path_docx}"
  [ "$status" -eq 0 ]

  "${docxbox}" txt "${path_docx}" | grep --invert-match --count "${pattern}"
}

title="With \"${base_command} newFilename.docx\" "
title+="text gets replaced by dummy text and is saved to new file"
@test "${title}" {
  path_docx_1="test/functional/tmp/cp_table_unordered_list_images.docx"
  path_docx_2="test/functional/tmp/lorem.docx"

  "${docxbox}" lorem "${path_docx_1}" "${path_docx_2}"
  ls test/functional/tmp | grep -c lorem.docx
}

@test "Output of \"docxbox lorem nonexistent.docx\" is an error message" {
  err_log="test/functional/tmp/err.log"

  run "${docxbox}" lorem nonexistent.docx
  [ "$status" -ne 0 ]

  "${docxbox}" lorem nonexistent.docx 2>&1 | tee "${err_log}"
  cat "${err_log}" | grep --count "docxBox Error - File not found:"
}

@test "Output of \"docxbox lorem wrong_file_type\" is an error message" {
  pattern="docxBox Error - File is no ZIP archive:"
  err_log="test/functional/tmp/err.log"
  wrong_file_types=(
  "test/functional/tmp/cp_lorem_ipsum.pdf"
  "test/functional/tmp/cp_mock_csv.csv"
  "test/functional/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    "${docxbox}" lorem "${i}" 2>&1 | tee "${err_log}"
    cat "${err_log}" | grep --count "${pattern}"
  done
}
