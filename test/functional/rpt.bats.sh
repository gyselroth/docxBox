#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

docxbox=""$BATS_TEST_DIRNAME"/docxbox"
path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

base_command="docxbox rpt filename.docx"

@test "Output of \"docxbox rpt {missing filename}\" is an error message" {
  run "${docxbox}" rpt
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: DOCX filename" = "${lines[0]}" ]
}

@test "Output of \"${base_command} {missing arguments}\" is an error message" {
  pattern="docxBox Error - Missing argument: String to be found (and replaced)"

  run "${docxbox}" rpt "${path_docx}"
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]
}

missing_argument="stringToBeReplaced {missing argument}"
@test "Output of \"${base_command} ${missing_argument}\" is an error message" {
  run "${docxbox}" rpt "${path_docx}" toBeReplaced
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: Replacement" = "${lines[0]}" ]
}

arguments="stringToBeReplaced replacementString"
appendix="the stringToBeReplaced gets replaced"
@test "With \"${base_command} ${arguments}\" ${appendix}" {
  run "${docxbox}" rpt "${path_docx}" Lorem Dorem
  [ "$status" -eq 0 ]
  "${docxbox}" txt "${path_docx}" | grep --count Dorem
}

arguments_new_docx="stringToBeReplaced replacementString newFile.docx"
appendix_new_docx="the stringToBeReplaced gets replaced and is saved to new file"
@test "With \"${base_command} ${arguments_new_docx}\" ${appendix_new_docx}" {
  path_docx_out="test/functional/tmp/replacedString.docx"

  run "${docxbox}" rpt "${path_docx}" Lorem Dorem "${path_docx_out}"
  [ "$status" -eq 0 ]
  "${docxbox}" txt "${path_docx_out}" | grep --count Dorem
}

@test "Output of \"docxbox rpt wrong_file_type\" is an error message" {
  pattern="docxBox Error - File is no ZIP archive:"
  err_log="test/functional/tmp/err.log"
  wrong_file_types=(
  "test/functional/tmp/cp_lorem_ipsum.pdf"
  "test/functional/tmp/cp_mock_csv.csv"
  "test/functional/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    "$BATS_TEST_DIRNAME"/docxbox rpt "${i}" Lorem Dorem 2>&1 | tee "${err_log}"
    cat "${err_log}" | grep --count "${pattern}"
  done
}
