#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper
source ./test/functional/_set_docxbox_binary.sh

CMD="docxbox lsd"

PATH_DOCX="test/tmp/cp_mergefields.docx"
ERR_LOG="test/tmp/err.log"

MERGE_FIELD="MERGEFIELD"
MERGE_FORMAT="\* MERGEFORMAT"

@test "${BATS_TEST_NUMBER}: Exit code of \"${CMD} filename.docx\" is zero" {
  run ${DOCXBOX_BINARY} lsd "${PATH_DOCX}"
  [ "$status" -eq 0 ]

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"${CMD} {missing argument}\" prints an error message" {
  run ${DOCXBOX_BINARY} lsd
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: Filename of DOCX to be extracted" = "${lines[0]}" ]

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx\" lists fields in docx" {
  ${DOCXBOX_BINARY} lsd "${PATH_DOCX}" | grep --count "${MERGE_FIELD}"
  ${DOCXBOX_BINARY} lsd "${PATH_DOCX}" | grep --count "${MERGE_FORMAT}"

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx\" lists fields in the header" {
  ${DOCXBOX_BINARY} lsd "${PATH_DOCX}" | grep --count "MERGEFIELD  Mergefield_Header"

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx\" lists fields in the footer" {
  ${DOCXBOX_BINARY} lsd "${PATH_DOCX}" | grep --count "MERGEFIELD  Mergefield_Footer"

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"docxbox ls filename.docx --fields\" lists fields in docx" {
  ${DOCXBOX_BINARY} lsd "${PATH_DOCX}" | grep --count "${MERGE_FIELD}"
  ${DOCXBOX_BINARY} lsd "${PATH_DOCX}" | grep --count "${MERGE_FORMAT}"

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"docxbox ls filename.docx -d\" lists fields in docx" {
  ${DOCXBOX_BINARY} ls "${PATH_DOCX}" -d | grep --count "${MERGE_FIELD}"
  ${DOCXBOX_BINARY} ls "${PATH_DOCX}" -d | grep --count "${MERGE_FORMAT}"

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"docxbox lsd nonexistent.docx\" prints an error message" {
  run ${DOCXBOX_BINARY} lsd nonexistent.docx
  [ "$status" -ne 0 ]

  source ./test/functional/_check_for_valgrind_errors.sh

  ${DOCXBOX_BINARY} lsd nonexistent.docx 2>&1 | tee "${ERR_LOG}"
  cat "${ERR_LOG}" | grep --count "docxBox Error - File not found:"
}

@test "${BATS_TEST_NUMBER}: \"${CMD} wrong_file_type\" prints an error message" {
  local wrong_file_types=(
  "test/tmp/cp_lorem_ipsum.pdf"
  "test/tmp/cp_mock_csv.csv"
  "test/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    ${DOCXBOX_BINARY} lsd "${i}" 2>&1 | tee "${ERR_LOG}"
    source ./test/functional/_check_for_valgrind_errors.sh
    cat "${ERR_LOG}" | grep --count "docxBox Error - File is no ZIP archive:"
  done
}
