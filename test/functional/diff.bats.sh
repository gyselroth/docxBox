#!/usr/bin/env bats
# shellcheck disable=SC1008

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

VALGRIND="valgrind -v --leak-check=full\
 --log-file=test/assets/documents/other/mem-leak.log"

if $IS_VALGRIND_TEST;
then
  DOCXBOX_BINARY="${VALGRIND} $BATS_TEST_DIRNAME/../tmp/docxbox"
else
  DOCXBOX_BINARY="$BATS_TEST_DIRNAME/../tmp/docxbox"
fi

PATH_DOCX_1="test/tmp/cp_table_unordered_list_images.docx"
PATH_DOCX_2="test/tmp/cp_bio_assay.docx"

base_command="docxbox diff filename.docx"
error_message="is an error message"

@test "Output of \"docxbox diff {missing argument}\" ${error_message}" {
  run ${DOCXBOX_BINARY} diff
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: DOCX file to compare with" = "${lines[0]}" ]
}

@test "Output of \"${base_command} {missing argument}\" ${error_message}" {
  path_docx="test/tmp/cp_table_unordered_list_images.docx"
  pattern="docxBox Error - Missing argument: DOCX file to compare with"

  run ${DOCXBOX_BINARY} diff "${path_docx}"
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]
}

title="Output of \"${base_command} otherFilename.docx {missing argument}\""
@test "${title} ${error_message}" {
  pattern="docxBox Error - Missing argument: File within DOCX archives to be compared"

  run ${DOCXBOX_BINARY} diff "${PATH_DOCX_1}" "${PATH_DOCX_2}"
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]
}

description="side by side view is displayed"
@test "With \"${base_command} changedFilename.docx\" a ${description}" {
  path_changed_docx="test/tmp/cp_table_unordered_list_images_v2.docx"
  
  run ${DOCXBOX_BINARY} lorem "${PATH_DOCX_1}" "${path_changed_docx}"

  result_original=$(${DOCXBOX_BINARY} cat "${PATH_DOCX_1}" word/document.xml | wc --bytes)
  result_changed=$(${DOCXBOX_BINARY} diff "${PATH_DOCX_1}" "${path_changed_docx}" word/document.xml | wc --bytes )
  
  ${DOCXBOX_BINARY} diff "${PATH_DOCX_1}" "${path_changed_docx}" word/document.xml | (( ${result_original} < ${result_changed} ))
}

@test "With \"${base_command} changedFilename.docx -u\" a unified ${description}" {
  path_changed_docx="test/tmp/cp_table_unordered_list_images_v2.docx"

  run ${DOCXBOX_BINARY} rpt "${PATH_DOCX_1}" Officia Foo "${path_changed_docx}"

  result_original=$(${DOCXBOX_BINARY} cat "${PATH_DOCX_1}" word/document.xml | wc --bytes)
  result_changed=$(${DOCXBOX_BINARY} diff "${PATH_DOCX_1}" "${path_changed_docx}" word/document.xml -u | wc --bytes )

  ${DOCXBOX_BINARY} diff "${PATH_DOCX_1}" "${path_changed_docx}" word/document.xml -u | (( ${result_original} > ${result_changed} ))
}

@test "Output of \"docxbox diff nonexistent.docx\" is an error message" {
  err_log="test/tmp/err.log"

  run ${DOCXBOX_BINARY} lsmj nonexistent.docx
  [ "$status" -ne 0 ]

  ${DOCXBOX_BINARY} lsmj nonexistent.docx 2>&1 | tee "${err_log}"
  cat "${err_log}" | grep --count "docxBox Error - File not found:"
}

@test "Output of \"docxbox diff wrongFileType\" is an error message" {
  pattern="docxBox Error - File is no ZIP archive:"
  err_log="test/tmp/err.log"
  wrong_file_types=(
  "test/tmp/cp_lorem_ipsum.pdf"
  "test/tmp/cp_mock_csv.csv"
  "test/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    ${DOCXBOX_BINARY} diff "${i}" "${PATH_DOCX_1}" word/document.xml 2>&1 | tee "${err_log}"
    cat "${err_log}" | grep --count "${pattern}"
  done
}
