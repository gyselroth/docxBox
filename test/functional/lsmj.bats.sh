#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

docxbox=""$BATS_TEST_DIRNAME"/docxbox"
path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

base_command="docxbox lsmj filename.docx"

description="contains information about the creation time and date"

pattern="\"created\": \"2017-08-02T11:09:18Z\""

@test "Exit code of \"${base_command}\" is zero" {
  run "${docxbox}" lsmj "${path_docx}"
  [ "$status" -eq 0 ]
}

@test "Output of \"docxbox lsmj {missing argument}\" is an error message" {
  run "${docxbox}" lsmj
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: Filename of DOCX to be extracted" = "${lines[0]}" ]
}

@test "Output of \"${base_command}\" contains information about the xml schema" {
  pattern="\"xmlSchema\": \"http://schemas.openxmlformats.org/officeDocument/2006\""

  "${docxbox}" lsmj "${path_docx}" | grep --count "${pattern}"
}

@test "Output of \"docxbox lsm filename.docx --json\" ${description}" {
  "${docxbox}" lsm "${path_docx}" --json | grep --count "${pattern}"
}

@test "Output of \"docxbox lsm filename.docx -j\" ${description}" {
  "${docxbox}" lsm "${path_docx}" -j | grep --count "${pattern}"
}

@test "Output of \"docxbox ls filename.docx --meta --json\" ${description}" {
  "${docxbox}" ls "${path_docx}" --meta --json | grep --count "${pattern}"
}

@test "Output of \"docxbox ls filename.docx -mj\" ${description}" {
  "${docxbox}" ls "${path_docx}" -mj | grep --count "${pattern}"
}

@test "Output of \"${base_command}\" contains language information" {
  pattern="\"language\": \"en-US\""

  "${docxbox}" lsmj "${path_docx}" | grep --count "${pattern}"
}

@test "Output of \"${base_command}\" contains information about the revision" {
  "${docxbox}" lsmj "${path_docx}" | grep --count "\"revision\": \"0\""
}

@test "Output of \"docxbox lsmj nonexistent.docx\" is an error message" {
  err_log="test/functional/tmp/err.log"

  run "${docxbox}" lsmj nonexistent.docx
  [ "$status" -ne 0 ]

  "${docxbox}" lsmj nonexistent.docx 2>&1 | tee "${err_log}"
  cat "${err_log}" | grep --count "docxBox Error - File not found:"
}

@test "Output of \"docxbox lsmj wrong_file_type\" is an error message" {
  pattern="docxBox Error - File is no ZIP archive:"
  err_log="test/functional/tmp/err.log"
  wrong_file_types=(
  "test/functional/tmp/cp_lorem_ipsum.pdf"
  "test/functional/tmp/cp_mock_csv.csv"
  "test/functional/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    "${docxbox}" lsmj "${i}" 2>&1 | tee "${err_log}"
    cat "${err_log}" | grep --count "${pattern}"
  done
}
