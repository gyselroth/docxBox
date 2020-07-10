#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper
source ./test/functional/_set_docxbox_binary.sh

CMD="docxbox lsdj"

PATH_DOCX="test/tmp/cp_table_unordered_list_images.docx"
ERR_LOG="test/tmp/err.log"

@test "${BATS_TEST_NUMBER}: Exit code of \"${CMD} filename.docx\" is zero" {
  run ${DOCXBOX_BINARY} lsdj "${PATH_DOCX}"
  [ "$status" -eq 0 ]

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"${CMD} {missing argument}\" prints an error message" {
  run ${DOCXBOX_BINARY} lsdj
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: Filename of DOCX to be extracted" = "${lines[0]}" ]

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"${CMD}\" lists fields in docx as JSON" {
  local pattern="table_unordered_list_images.docx-"
  ${DOCXBOX_BINARY} lsdj "${PATH_DOCX}" | grep --count "${pattern}"

  source ./test/functional/_check_for_valgrind_errors.sh

  local pattern="/word/document.xml"
  ${DOCXBOX_BINARY} lsdj "${PATH_DOCX}" | grep --count "${pattern}"

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"docxbox ls filename.docx --fields --json\" lists fields in docx as JSON" {
  local pattern="table_unordered_list_images.docx-"
  ${DOCXBOX_BINARY} ls "${PATH_DOCX}" --fields --json | grep --count "${pattern}"

  source ./test/functional/_check_for_valgrind_errors.sh

  local pattern="/word/document.xml"
  ${DOCXBOX_BINARY} ls "${PATH_DOCX}" --fields --json | grep --count "${pattern}"

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"docxbox ls filename.docx -dj\" lists fields in docx as JSON" {
  local pattern="table_unordered_list_images.docx-"
  ${DOCXBOX_BINARY} ls "${PATH_DOCX}" -dj | grep --count "${pattern}"

  local pattern="/word/document.xml"
  ${DOCXBOX_BINARY} ls "${PATH_DOCX}" -dj | grep --count "${pattern}"

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"docxbox lsd filename.docx --json\" lists fields in docx as JSON" {
  local pattern="table_unordered_list_images.docx-"
  ${DOCXBOX_BINARY} lsd "${PATH_DOCX}" --json | grep --count "${pattern}"

  source ./test/functional/_check_for_valgrind_errors.sh

  local pattern="/word/document.xml"
  ${DOCXBOX_BINARY} lsd "${PATH_DOCX}" --json | grep --count "${pattern}"

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"${CMD} nonexistent.docx\" prints an error message" {
  run ${DOCXBOX_BINARY} lsdj nonexistent.docx
  [ "$status" -ne 0 ]
  source ./test/functional/_check_for_valgrind_errors.sh

  ${DOCXBOX_BINARY} lsdj nonexistent.docx 2>&1 | tee "${ERR_LOG}"
  cat "${ERR_LOG}" | grep --count "docxBox Error - File not found:"
}

@test "${BATS_TEST_NUMBER}: \"${CMD} wrong_file_type\" prints an error message" {
  local wrong_file_types=(
  "test/tmp/cp_lorem_ipsum.pdf"
  "test/tmp/cp_mock_csv.csv"
  "test/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    ${DOCXBOX_BINARY} lsdj "${i}" 2>&1 | tee "${ERR_LOG}"
    source ./test/functional/_check_for_valgrind_errors.sh
    cat "${ERR_LOG}" | grep --count "docxBox Error - File is no ZIP archive:"
  done
}

@test "${BATS_TEST_NUMBER}: First char of \"${CMD} filename.docx\"'s JSON is a \"{\"" {
  ${DOCXBOX_BINARY} lsdj "${PATH_DOCX}" | grep "^[{]"
}

@test "${BATS_TEST_NUMBER}: Last char of \"${CMD} filename.docx\"'s JSON is a \"}\"" {
  ${DOCXBOX_BINARY} lsdj "${PATH_DOCX}" | grep "[}]$"
}

@test "${BATS_TEST_NUMBER}: Amount opening and closing brackets \"[]\" must match" {
  local amount_opening=$(${DOCXBOX_BINARY} lsdj "${PATH_DOCX}" | grep --count "\[")
  local amount_closing=$(${DOCXBOX_BINARY} lsdj "${PATH_DOCX}" | grep --count "\]")

  (( amount_opening == amount_closing ))
}

@test "${BATS_TEST_NUMBER}: Amount opening and closing brackets \"{}\" must match" {
  local amount_opening=$(${DOCXBOX_BINARY} lsdj "${PATH_DOCX}" | grep --count "\{")
  local amount_closing=$(${DOCXBOX_BINARY} lsdj "${PATH_DOCX}" | grep --count "\}")

  (( amount_opening == amount_closing ))
}
