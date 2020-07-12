#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper
source ./test/functional/_set_docxbox_binary.sh

CMD="docxbox lsmj"

PATH_DOCX="test/tmp/cp_table_unordered_list_images.docx"
ERR_LOG="test/tmp/err.log"

PATTERN_CREATED="\"created\": \"2020-06-18T10:30:11Z\""

@test "${BATS_TEST_NUMBER}: Exit code of \"${CMD} filename.docx\" is zero" {
  run ${DOCXBOX_BINARY} lsmj "${PATH_DOCX}"
  [ "$status" -eq 0 ]

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"${CMD} {missing argument}\" prints an error message" {
  local pattern="docxBox Error - Missing argument: Filename of DOCX to be extracted"

  run ${DOCXBOX_BINARY} lsmj
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx\" displays xml schema information" {
  local pattern="\"xmlSchema\": \"http://schemas.openxmlformats.org/officeDocument/2006\""

  ${DOCXBOX_BINARY} lsmj "${PATH_DOCX}" | grep --count "${pattern}"

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"docxbox lsm filename.docx --json\" displays creation time/date information" {
  ${DOCXBOX_BINARY} lsm "${PATH_DOCX}" --json | grep --count "${PATTERN_CREATED}"

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"docxbox lsm filename.docx -j\" displays creation time/date information" {
  ${DOCXBOX_BINARY} lsm "${PATH_DOCX}" -j | grep --count "${PATTERN_CREATED}"

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"docxbox ls filename.docx --meta --json\" displays creation time/date information" {
  ${DOCXBOX_BINARY} ls "${PATH_DOCX}" --meta --json | grep --count "${PATTERN_CREATED}"

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"docxbox ls filename.docx -mj\" displays creation time/date information" {
  ${DOCXBOX_BINARY} ls "${PATH_DOCX}" -mj | grep --count "${PATTERN_CREATED}"

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx\" displays language information" {
  local pattern="\"language\": \"en-US\""

  ${DOCXBOX_BINARY} lsmj "${PATH_DOCX}" | grep --count "${pattern}"

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx\" displays revision information" {
  ${DOCXBOX_BINARY} lsmj "${PATH_DOCX}" | grep --count "\"revision\": \"2\""

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"${CMD} nonexistent.docx\" prints an error message" {
  run ${DOCXBOX_BINARY} lsmj nonexistent.docx
  [ "$status" -ne 0 ]

  source ./test/functional/_check_for_valgrind_errors.sh

  ${DOCXBOX_BINARY} lsmj nonexistent.docx 2>&1 | tee "${ERR_LOG}"
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
    ${DOCXBOX_BINARY} lsmj "${i}" 2>&1 | tee "${ERR_LOG}"
    source ./test/functional/_check_for_valgrind_errors.sh
    cat "${ERR_LOG}" | grep --count "${pattern}"
  done
}

@test "${BATS_TEST_NUMBER}: First char of \"${CMD} filename.docx\"'s JSON is a \"{}\"" {
  ${DOCXBOX_BINARY} lsmj "${PATH_DOCX}" | grep "^[{]"
}

@test "${BATS_TEST_NUMBER}: Last char of \"${CMD} filename.docx\"'s JSON is a \"}\"" {
  ${DOCXBOX_BINARY} lsmj "${PATH_DOCX}" | grep "[}]$"
}

@test "${BATS_TEST_NUMBER}: Amount opening and closing brackets \"{}\" must match" {
  local amount_opening amount_closing
  amount_opening=$(${DOCXBOX_BINARY} lsmj "${PATH_DOCX}" | grep --count "\{")
  amount_closing=$(${DOCXBOX_BINARY} lsmj "${PATH_DOCX}" | grep --count "\}")

  (( amount_opening = amount_closing ))
}
