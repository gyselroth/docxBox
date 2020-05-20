#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

docxbox=""$BATS_TEST_DIRNAME"/docxbox"
path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

base_command="docxbox lsij filename.docx"
description="are contained images as JSON"

@test "Exit code of \"${base_command}\" is zero" {
  run "${docxbox}" lsij "${path_docx}"
  [ "$status" -eq 0 ]
}

@test "Output of \"docxbox lsij {missing argument}\" is an error message" {
  run "${docxbox}" lsij
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: DOCX filename" = "${lines[0]}" ]
}

@test "Output of \"${base_command}\" is contained images as JSON" {
  "${docxbox}" lsij "${path_docx}" | grep --count "image1.jpeg"
}

@test "Output of \"docxbox lsi filename.docx --json\" ${description}" {
  "${docxbox}" lsi "${path_docx}" --json | grep --count "image1.jpeg"
}

@test "Output of \"docxbox lsi filename.docx -j\" ${description}" {
  "${docxbox}" lsi "${path_docx}" -j | grep --count "image1.jpeg"
}

@test "Output of \"docxbox ls filename.docx -ij\" ${description}" {
  "${docxbox}" lsi "${path_docx}" -ij | grep --count "image1.jpeg"
}

@test "Output of \"docxbox ls filename.docx --images --json\" ${description}" {
  "${docxbox}" ls "${path_docx}" --images --json | grep --count "image1.jpeg"
}

@test "Output of \"docxbox lsij wrong_file_type\" is an error message" {
  err_log="test/functional/tmp/err.log"
  wrong_file_types=(
  "test/functional/tmp/cp_lorem_ipsum.pdf"
  "test/functional/tmp/cp_mock_csv.csv"
  "test/functional/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    "${docxbox}" lsij "${i}" 2>&1 | tee "${err_log}"
    cat "${err_log}" | grep --count "docxBox Error - File is no ZIP archive:"
  done
}
