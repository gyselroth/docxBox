#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

docxbox="$BATS_TEST_DIRNAME/docxbox"
base_command="docxbox lsdj filename.docx"

path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"
long_description_json="the fields in the docx are listed as JSON"

@test "Exit code of \"${base_command}\" is zero" {
  run "${docxbox}" lsdj "${path_docx}"
  [ "$status" -eq 0 ]
}

@test "Output of \"docxbox lsdj {missing argument}\" is an error message" {
  run "${docxbox}" lsdj
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: Filename of DOCX to be extracted" = "${lines[0]}" ]
}

@test "With \"${base_command}\" the fields in the docx are listed as JSON" {
  pattern="table_unordered_list_images.docx-"
  "${docxbox}" lsdj "${path_docx}" | grep --count "${pattern}"

  pattern="/word/document.xml"
  "${docxbox}" lsdj "${path_docx}" | grep --count "${pattern}"
}

longhand="--fields --json"
title="With \"docxbox ls filename.docx ${longhand}\" "
title+="${long_description_json}"
@test "${title}" {
  pattern="table_unordered_list_images.docx-"
  "${docxbox}" ls "${path_docx}" ${longhand} | grep --count "${pattern}"

  pattern="/word/document.xml"
  "${docxbox}" ls "${path_docx}" ${longhand} | grep --count "${pattern}"
}

@test "With \"docxbox ls filename.docx -dj\" ${long_description_json}" {
  pattern="table_unordered_list_images.docx-"
  "${docxbox}" ls "${path_docx}" -dj | grep --count "${pattern}"

  pattern="/word/document.xml"
  "${docxbox}" ls "${path_docx}" -dj | grep --count "${pattern}"
}

@test "With \"docxbox lsd filename.docx --json\" ${long_description_json}" {
  pattern="table_unordered_list_images.docx-"
  "${docxbox}" lsd "${path_docx}" --json | grep --count "${pattern}"

  pattern="/word/document.xml"
  "${docxbox}" lsd "${path_docx}" --json | grep --count "${pattern}"
}

@test "Output of \"docxbox lsdj nonexistent.docx\" is an error message" {
  err_log="test/functional/tmp/err.log"

  run "$BATS_TEST_DIRNAME"/docxbox lsdj nonexistent.docx
  [ "$status" -ne 0 ]

  "$BATS_TEST_DIRNAME"/docxbox lsdj nonexistent.docx 2>&1 | tee "${err_log}"
  cat "${err_log}" | grep --count "docxBox Error - File not found:"
}

@test "Output of \"${base_command} wrong_file_type\" is an error message" {
  err_log="test/functional/tmp/err.log"
  wrong_file_types=(
  "test/functional/tmp/cp_lorem_ipsum.pdf"
  "test/functional/tmp/cp_mock_csv.csv"
  "test/functional/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    "${docxbox}" lsdj "${i}" 2>&1 | tee "${err_log}"
    cat "${err_log}" | grep --count "docxBox Error - File is no ZIP archive:"
  done
}

