#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper
source ./test/functional/_set_docxbox_binary.sh

CMD="docxbox lsm"

PATH_DOCX="test/tmp/cp_table_unordered_list_images.docx"
ERR_LOG="test/tmp/err.log"

@test "${BATS_TEST_NUMBER}: Exit code of \"${CMD} filename.docx\" is zero" {
  run "${DOCXBOX_BINARY}" lsm "${PATH_DOCX}"
  [ "$status" -eq 0 ]

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"${CMD} {missing argument}\" prints an error message" {
  run "${DOCXBOX_BINARY}" lsm
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: Filename of DOCX to be extracted" = "${lines[0]}" ]

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx\" displays xml schema information" {
  local xml_schema="xmlSchema: http://schemas.openxmlformats.org/officeDocument/2006"

  ${DOCXBOX_BINARY} lsm "${PATH_DOCX}" | grep --count "${xml_schema}"

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx\" displays creation time/date information" {
  local created="created: \d{4,4}-\d{2,2}-\d{1,2}T\d{1,2}:\d{1,2}:\d{1,2}Z"

  ${DOCXBOX_BINARY} lsm "${PATH_DOCX}" | grep --count --perl-regexp "${created}"

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx\" displays language information" {
  ${DOCXBOX_BINARY} lsm "${PATH_DOCX}" | grep --count "language: en-US"

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx\" displays revision information" {
  ${DOCXBOX_BINARY} lsm "${PATH_DOCX}" | grep --count "revision: 2"

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"${CMD} nonexistent.docx\" prints an error message" {
  run "${DOCXBOX_BINARY}" lsm nonexistent.docx
  [ "$status" -ne 0 ]

  source ./test/functional/_check_for_valgrind_errors.sh

  ${DOCXBOX_BINARY} lsm nonexistent.docx 2>&1 | tee "${ERR_LOG}"
  cat "${ERR_LOG}" | grep --count "docxBox Error - File not found:"
}

@test "${BATS_TEST_NUMBER}: \"${CMD} wrong_file_type\" prints an error message" {
  local pattern="docxBox Error - File is no ZIP archive:"
  local wrong_file_types=(
  "test/tmp/cp_lorem_ipsum.pdf"
  "test/tmp/cp_mock_csv.csv"
  "test/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    ${DOCXBOX_BINARY} lsm "${i}" 2>&1 | tee "${ERR_LOG}"
    source ./test/functional/_check_for_valgrind_errors.sh
    cat "${ERR_LOG}" | grep --count "${pattern}"
  done
}
