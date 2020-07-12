#!/usr/bin/env bats
# shellcheck disable=SC1008

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper
source ./test/functional/_set_docxbox_binary.sh

CMD="docxbox diff"

PATH_DOCX_1="test/tmp/cp_table_unordered_list_images.docx"
PATH_DOCX_2="test/tmp/cp_bio_assay.docx"
ERR_LOG="test/tmp/err.log"

@test "$BATS_TEST_NUMBER: \"${CMD} filename.docx {missing argument}\" prints an error message" {
  run "${DOCXBOX_BINARY}" diff
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: DOCX file to compare with" = "${lines[0]}" ]

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx {missing argument}\" prints an error message" {
  local path_docx="test/tmp/cp_table_unordered_list_images.docx"
  local pattern="docxBox Error - Missing argument: DOCX file to compare with"

  run "${DOCXBOX_BINARY}" diff "${path_docx}"
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx otherFilename.docx {missing argument}\" prints an error message " {
  local pattern="docxBox Error - Missing argument: File within DOCX archives to be compared"

  run "${DOCXBOX_BINARY}" diff "${PATH_DOCX_1}" "${PATH_DOCX_2}"
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename_v1.docx filename_v2.docx compare_file\" displays a side by side view" {
  local path_changed_docx="test/tmp/cp_table_unordered_list_images_v2.docx"

  run "${DOCXBOX_BINARY}" lorem "${PATH_DOCX_1}" "${path_changed_docx}"

  local result_original result_changed
  result_original=$(${DOCXBOX_BINARY} cat "${PATH_DOCX_1}" word/document.xml | wc --bytes)
  result_changed=$(${DOCXBOX_BINARY} diff "${PATH_DOCX_1}" "${path_changed_docx}" word/document.xml | wc --bytes )

  (( result_original < result_changed ))

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"${CMD} changedFilename.docx -u\" displays a unified side by side view" {
  local path_changed_docx="test/tmp/cp_table_unordered_list_images_v2.docx"

  run "${DOCXBOX_BINARY}" rpt "${PATH_DOCX_1}" Officia Foo "${path_changed_docx}"

  local result_original result_changed
  result_original=$(${DOCXBOX_BINARY} cat "${PATH_DOCX_1}" word/document.xml | wc --bytes)
  result_changed=$(${DOCXBOX_BINARY} diff "${PATH_DOCX_1}" "${path_changed_docx}" word/document.xml -u | wc --bytes )

  (( result_original != result_changed ))

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"${CMD} nonexistent.docx\" prints an error message" {
  run "${DOCXBOX_BINARY}" diff nonexistent.docx nonexistent.docx
  [ "$status" -ne 0 ]

  ${DOCXBOX_BINARY} diff nonexistent.docx nonexistent.docx word/document.xml 2>&1 | tee "${ERR_LOG}"
  cat "${ERR_LOG}" | grep --count "docxBox Error - File not found:"

  source ./test/functional/_check_for_valgrind_errors.sh
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
    source ./test/functional/_check_for_valgrind_errors.sh
    cat "${ERR_LOG}" | grep --count "${pattern}"
  done
}
