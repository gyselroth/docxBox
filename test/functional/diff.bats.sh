#!/usr/bin/env bats
# shellcheck disable=SC1008

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

docxbox=""$BATS_TEST_DIRNAME"/docxbox"

path_docx_1="test/functional/tmp/cp_table_unordered_list_images.docx"
path_docx_2="test/functional/tmp/cp_bio_assay.docx"

base_command="docxbox diff filename.docx"
error_message="is an error message"

@test "Output of \"docxbox diff {missing argument}\" ${error_message}" {
  run "${docxbox}" diff
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: DOCX file to compare with" = "${lines[0]}" ]
}

@test "Output of \"${base_command} {missing argument}\" ${error_message}" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"
  pattern="docxBox Error - Missing argument: DOCX file to compare with"

  run "${docxbox}" diff "${path_docx}"
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]
}

title="Output of \"${base_command} otherFilename.docx {missing argument}\""
@test "${title} ${error_message}" {
  pattern="docxBox Error - Missing argument: File within DOCX archives to be compared"

  run "${docxbox}" diff "${path_docx_1}" "${path_docx_2}"
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]
}

description="side by side view is displayed"
@test "With \"${base_command} changedFilename.docx\" a ${description}" {
  path_changed_docx="test/functional/tmp/cp_table_unordered_list_images_v2.docx"
  
  run "${docxbox}" lorem "${path_docx_1}" "${path_changed_docx}"

  result_original=$("${docxbox}" cat "${path_docx_1}" word/document.xml | wc --bytes)
  result_changed=$("${docxbox}" diff "${path_docx_1}" "${path_changed_docx}" word/document.xml | wc --bytes )
  
  "${docxbox}" diff "${path_docx_1}" "${path_changed_docx}" word/document.xml | (( ${result_original} < ${result_changed} ))
}

@test "With \"${base_command} changedFilename.docx -u\" a unified ${description}" {
  path_changed_docx="test/functional/tmp/cp_table_unordered_list_images_v2.docx"

  run "${docxbox}" rpt "${path_docx_1}" Officia Foo "${path_changed_docx}"

  result_original=$("${docxbox}" cat "${path_docx_1}" word/document.xml | wc --bytes)
  result_changed=$("${docxbox}" diff "${path_docx_1}" "${path_changed_docx}" word/document.xml -u | wc --bytes )

  "${docxbox}" diff "${path_docx_1}" "${path_changed_docx}" word/document.xml -u | (( ${result_original} > ${result_changed} ))
}

@test "Output of \"docxbox diff nonexistent.docx\" is an error message" {
  err_log="test/functional/tmp/err.log"

  run "${docxbox}" lsmj nonexistent.docx
  [ "$status" -ne 0 ]

  "${docxbox}" lsmj nonexistent.docx 2>&1 | tee "${err_log}"
  cat "${err_log}" | grep --count "docxBox Error - File not found:"
}

@test "Output of \"docxbox diff wrongFileType\" is an error message" {
  pattern="docxBox Error - File is no ZIP archive:"
  err_log="test/functional/tmp/err.log"
  wrong_file_types=(
  "test/functional/tmp/cp_lorem_ipsum.pdf"
  "test/functional/tmp/cp_mock_csv.csv"
  "test/functional/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    "${docxbox}" diff "${i}" "${path_docx_1}" word/document.xml 2>&1 | tee "${err_log}"
    cat "${err_log}" | grep --count "${pattern}"
  done
}
