#!/usr/bin/env bats
# shellcheck disable=SC1008

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

CMD="docxbox diff"

VALGRIND_LOG="test/tmp/mem-leak.log"
VALGRIND="valgrind -v --leak-check=full\
 --log-file=${VALGRIND_LOG}"

VALGRIND_ERR_PATTERN="ERROR SUMMARY: [1-9] errors from [1-9] contexts"

if $IS_VALGRIND_TEST;
then
  DOCXBOX_BINARY="${VALGRIND} $BATS_TEST_DIRNAME/../tmp/docxbox"
else
  DOCXBOX_BINARY="$BATS_TEST_DIRNAME/../tmp/docxbox"
fi

PATH_DOCX_1="test/tmp/cp_table_unordered_list_images.docx"
PATH_DOCX_2="test/tmp/cp_bio_assay.docx"
ERR_LOG="test/tmp/err.log"

ERROR_MESSAGE="is an error message"

@test "$BATS_TEST_NUMBER: \"${CMD} filename.docx {missing argument}\" prints an error message" {
  run ${DOCXBOX_BINARY} diff
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: DOCX file to compare with" = "${lines[0]}" ]

  check_for_valgrind_error
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx {missing argument}\" prints an error message" {
  local path_docx="test/tmp/cp_table_unordered_list_images.docx"
  local pattern="docxBox Error - Missing argument: DOCX file to compare with"

  run ${DOCXBOX_BINARY} diff "${path_docx}"
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]

  check_for_valgrind_error
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx otherFilename.docx {missing argument}\" prints an error message " {
  local pattern="docxBox Error - Missing argument: File within DOCX archives to be compared"

  run ${DOCXBOX_BINARY} diff "${PATH_DOCX_1}" "${PATH_DOCX_2}"
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]

  check_for_valgrind_error
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename_v1.docx filename_v2.docx compare_file\" displays a side by side view" {
  local path_changed_docx="test/tmp/cp_table_unordered_list_images_v2.docx"
  
  run ${DOCXBOX_BINARY} lorem "${PATH_DOCX_1}" "${path_changed_docx}"

  local result_original=$(${DOCXBOX_BINARY} cat "${PATH_DOCX_1}" word/document.xml | wc --bytes)
  local result_changed=$(${DOCXBOX_BINARY} diff "${PATH_DOCX_1}" "${path_changed_docx}" word/document.xml | wc --bytes )
  
  (( result_original < result_changed ))

  check_for_valgrind_error
}

@test "${BATS_TEST_NUMBER}: \"${CMD} changedFilename.docx -u\" displays a unified side by side view" {
  local path_changed_docx="test/tmp/cp_table_unordered_list_images_v2.docx"

  run ${DOCXBOX_BINARY} rpt "${PATH_DOCX_1}" Officia Foo "${path_changed_docx}"

  local result_original=$(${DOCXBOX_BINARY} cat "${PATH_DOCX_1}" word/document.xml | wc --bytes)
  local result_changed=$(${DOCXBOX_BINARY} diff "${PATH_DOCX_1}" "${path_changed_docx}" word/document.xml -u | wc --bytes )

  (( result_original != result_changed ))

  check_for_valgrind_error
}

@test "${BATS_TEST_NUMBER}: \"${CMD} nonexistent.docx\" prints an error message" {
  run ${DOCXBOX_BINARY} diff nonexistent.docx nonexistent.docx
  [ "$status" -ne 0 ]

  ${DOCXBOX_BINARY} diff nonexistent.docx nonexistent.docx word/document.xml 2>&1 | tee "${ERR_LOG}"
  cat "${ERR_LOG}" | grep --count "docxBox Error - File not found:"

  check_for_valgrind_error
}

@test "${BATS_TEST_NUMBER}: \"${CMD} wrongFileType\" prints an error message" {
  local pattern="docxBox Error - File is no ZIP archive:"
  local wrong_file_types=(
  "test/tmp/cp_lorem_ipsum.pdf"
  "test/tmp/cp_mock_csv.csv"
  "test/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    ${DOCXBOX_BINARY} diff "${i}" "${PATH_DOCX_1}" word/document.xml 2>&1 | tee "${ERR_LOG}"
    check_for_valgrind_error
    cat "${ERR_LOG}" | grep --count "${pattern}"
  done
}
