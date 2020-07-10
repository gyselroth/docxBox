#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper
source ./test/functional/_set_docxbox_binary.sh

CMD="docxbox lsl"

PATH_DOCX="test/tmp/cp_table_unordered_list_images.docx"
ERR_LOG="test/tmp/err.log"

REGEX="/[0-9A-Z]\{8\}/"

SEARCH_RESULTS=(
"word/document.xml"
"word/fontTable.xml"
"word/numbering.xml"
"word/styles.xml")

REGEX_RESULT="docProps/core.xml"

@test "${BATS_TEST_NUMBER}: Exit code of \"${CMD} filename.docx searchString\" is zero" {
  run ${DOCXBOX_BINARY} lsl "${PATH_DOCX}" fonts
  [ "$status" -eq 0 ]

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"${CMD} {missing argument}\" prints an error message" {
  run ${DOCXBOX_BINARY} lsl
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: DOCX filename" = "${lines[0]}" ]

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx {missing argument}\" prints an error message" {
  local pattern="docxBox Error - Missing argument: \
String or regular expression to be located"

  run ${DOCXBOX_BINARY} lsl "${PATH_DOCX}"
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx searchString\" lists all files containing given searchString" {
  for i in "${SEARCH_RESULTS[@]}"
  do
    ${DOCXBOX_BINARY} lsl "${PATH_DOCX}" fonts | grep --count "${i}"
    source ./test/functional/_check_for_valgrind_errors.sh
  done
}

@test "${BATS_TEST_NUMBER}: \"docxbox ls filename.docx -l searchString\" lists all files containing given searchString" {
  for i in "${SEARCH_RESULTS[@]}"
  do
    ${DOCXBOX_BINARY} ls "${PATH_DOCX}" -l fonts | grep --count "${i}"
    source ./test/functional/_check_for_valgrind_errors.sh
  done
}

@test "${BATS_TEST_NUMBER}: \"docxbox ls filename.docx --locate searchString\" lists all files containing given searchString" {
  for i in "${SEARCH_RESULTS[@]}"
  do
    ${DOCXBOX_BINARY} lsl "${PATH_DOCX}" --locate fonts | grep --count "${i}"
    source ./test/functional/_check_for_valgrind_errors.sh
  done
}

@test "${BATS_TEST_NUMBER}: \"docxbox lsl filename.docx REGEX\" lists all files containing given regular expression" {
  ${DOCXBOX_BINARY} lsl "${PATH_DOCX}" "${REGEX}" | grep --count ${REGEX_RESULT}

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"docxbox ls filename.docx -l REGEX\" lists all files containing given regular expression" {
  ${DOCXBOX_BINARY} ls "${PATH_DOCX}" -l "${REGEX}" | grep --count ${REGEX_RESULT}

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"docxbox ls filename.docx --locate REGEX\" lists all files containing given regular expression" {
  ${DOCXBOX_BINARY} ls "${PATH_DOCX}" --locate "${REGEX}" | grep --count ${REGEX_RESULT}

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"${CMD} nonexistent.docx searchString\" prints an error message" {
  run ${DOCXBOX_BINARY} lsl nonexistent.docx fonts
  [ "$status" -ne 0 ]

  source ./test/functional/_check_for_valgrind_errors.sh

  ${DOCXBOX_BINARY} lsl nonexistent.docx fonts 2>&1 | tee "${ERR_LOG}"
  cat "${ERR_LOG}" | grep --count "docxBox Error - File not found:"
}

@test "${BATS_TEST_NUMBER}: \"${CMD} wrong_file_type\" prints an error message" {
  local wrong_file_types=(
  "test/tmp/cp_lorem_ipsum.pdf"
  "test/tmp/cp_mock_csv.csv"
  "test/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    ${DOCXBOX_BINARY} lsl "${i}" fonts 2>&1 | tee "${ERR_LOG}"
    source ./test/functional/_check_for_valgrind_errors.sh
    cat "${ERR_LOG}" | grep --count "docxBox Error - File is no ZIP archive:"
  done
}
