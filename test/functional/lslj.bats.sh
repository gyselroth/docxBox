#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper
source ./test/functional/_set_docxbox_binary.sh

CMD="docxbox lslj"

PATH_DOCX="test/tmp/cp_table_unordered_list_images.docx"
ERR_LOG="test/tmp/err.log"

SEARCH_RESULTS=(
"\"file\":\"word/document.xml\""
"\"file\":\"word/fontTable.xml\""
"\"file\":\"word/numbering.xml\""
"\"file\":\"word/styles.xml\"")

@test "${BATS_TEST_NUMBER}: \"${CMD} {missing argument}\" prints an error message" {
  run "${DOCXBOX_BINARY}" lslj
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: DOCX filename" = "${lines[0]}" ]
  
  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx {missing argument}\" prints an error message" {
  local pattern="docxBox Error - Missing argument: \
String or regular expression to be located"

  run "${DOCXBOX_BINARY}" lslj "${PATH_DOCX}"
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]
  
  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx searchString\" lists all files containing given searchString" {
  for i in "${SEARCH_RESULTS[@]}"
  do
    ${DOCXBOX_BINARY} lslj "${PATH_DOCX}" fonts | grep -c "${i}"
    source ./test/functional/_check_for_valgrind_errors.sh
  done
}

@test "${BATS_TEST_NUMBER}: \"docxbox lsl filename.docx -j searchString\" lists all files containing given searchString" {
  for i in "${SEARCH_RESULTS[@]}"
  do
    ${DOCXBOX_BINARY} lsl "${PATH_DOCX}" -j fonts | grep -c "${i}"
    source ./test/functional/_check_for_valgrind_errors.sh
  done
}

@test "${BATS_TEST_NUMBER}: \"docxbox lsl filename.docx --json searchString\" lists all files containing given searchString" {
  for i in "${SEARCH_RESULTS[@]}"
  do
    ${DOCXBOX_BINARY} lsl "${PATH_DOCX}" --json fonts | grep -c "${i}"
    source ./test/functional/_check_for_valgrind_errors.sh
  done
}

@test "${BATS_TEST_NUMBER}: \"docxbox ls filename.docx -lj searchString\" lists all files containing given searchString" {
  for i in "${SEARCH_RESULTS[@]}"
  do
    ${DOCXBOX_BINARY} ls "${PATH_DOCX}" -lj fonts | grep -c "${i}"
    source ./test/functional/_check_for_valgrind_errors.sh
  done
}

@test "${BATS_TEST_NUMBER}: \"docxbox ls filename.docx --locate -j searchString\" lists all files containing given searchString" {
  for i in "${SEARCH_RESULTS[@]}"
  do
    ${DOCXBOX_BINARY} ls "${PATH_DOCX}" --locate -j fonts | grep -c "${i}"
    source ./test/functional/_check_for_valgrind_errors.sh
  done
}

@test "${BATS_TEST_NUMBER}: \"docxbox ls filename.docx --locate --json searchString\" lists all files containing given searchString" {
  for i in "${SEARCH_RESULTS[@]}"
  do
    ${DOCXBOX_BINARY} ls "${PATH_DOCX}" --locate --json fonts | grep -c "${i}"
    source ./test/functional/_check_for_valgrind_errors.sh
  done
}

@test "${BATS_TEST_NUMBER}: \"${CMD} nonexistent.docx searchString\" prints an error message" {
  run "${DOCXBOX_BINARY}" lslj nonexistent.docx fonts
  [ "$status" -ne 0 ]

  source ./test/functional/_check_for_valgrind_errors.sh

  ${DOCXBOX_BINARY} lslj nonexistent.docx fonts 2>&1 | tee "${ERR_LOG}"
  cat "${ERR_LOG}" | grep --count "docxBox Error - File not found:"
}

@test "${BATS_TEST_NUMBER}: \"${CMD} wrong_file_type searchString\" prints an error message" {
  local wrong_file_types=(
  "test/tmp/cp_lorem_ipsum.pdf"
  "test/tmp/cp_mock_csv.csv"
  "test/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    ${DOCXBOX_BINARY} lslj "${i}" fonts 2>&1 | tee "${ERR_LOG}"
    source ./test/functional/_check_for_valgrind_errors.sh
    cat "${ERR_LOG}" | grep --count "docxBox Error - File is no ZIP archive:"
  done
}

@test "${BATS_TEST_NUMBER}: First char of \"${CMD} filename.docx\"'s JSON is a \"[\"" {
  ${DOCXBOX_BINARY} lslj "${PATH_DOCX}" fonts | grep "^[[]"
}

@test "${BATS_TEST_NUMBER}: Last char of \"${CMD} filename.docx\"'s JSON is a \"]\"" {
  ${DOCXBOX_BINARY} lslj "${PATH_DOCX}" fonts | grep "[]]$"
}

@test "${BATS_TEST_NUMBER}: Amount opening and closing brackets \"[]\" must match" {
  local amount_opening amount_closing
  amount_opening=$(${DOCXBOX_BINARY} lslj "${PATH_DOCX}" fonts | grep --count "\[")
  amount_closing=$(${DOCXBOX_BINARY} lslj "${PATH_DOCX}" fonts | grep --count "\]")

  (( amount_opening = amount_closing ))
}

@test "${BATS_TEST_NUMBER}: Amount opening and closing brackets \"{}\" must match" {
  local amount_opening amount_closing
  amount_opening=$(${DOCXBOX_BINARY} lslj "${PATH_DOCX}" fonts | grep --count "\{")
  amount_closing=$(${DOCXBOX_BINARY} lslj "${PATH_DOCX}" fonts | grep --count "\}")

  (( amount_opening = amount_closing ))
}
