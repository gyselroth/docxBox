#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

CMD="docxbox lslj"

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

PATH_DOCX="test/tmp/cp_table_unordered_list_images.docx"
ERR_LOG="test/tmp/err.log"

SEARCH_RESULTS=(
"\"file\":\"word/document.xml\""
"\"file\":\"word/fontTable.xml\""
"\"file\":\"word/numbering.xml\""
"\"file\":\"word/styles.xml\"")

@test "${BATS_TEST_NUMBER}: \"${CMD} {missing argument}\" prints an error message" {
  run ${DOCXBOX_BINARY} lslj
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: DOCX filename" = "${lines[0]}" ]
  
  check_for_valgrind_error
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx {missing argument}\" prints an error message" {
  local pattern="docxBox Error - Missing argument: \
String or regular expression to be located"

  run ${DOCXBOX_BINARY} lslj "${PATH_DOCX}"
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]
  
  check_for_valgrind_error
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx searchString\" lists all files containing given searchString" {
  for i in "${SEARCH_RESULTS[@]}"
  do
    ${DOCXBOX_BINARY} lslj "${PATH_DOCX}" fonts | grep -c "${i}"
    check_for_valgrind_error
  done
}

@test "${BATS_TEST_NUMBER}: \"docxbox lsl filename.docx -j searchString\" lists all files containing given searchString" {
  for i in "${SEARCH_RESULTS[@]}"
  do
    ${DOCXBOX_BINARY} lsl "${PATH_DOCX}" -j fonts | grep -c "${i}"
    check_for_valgrind_error
  done
}

@test "${BATS_TEST_NUMBER}: \"docxbox lsl filename.docx --json searchString\" lists all files containing given searchString" {
  for i in "${SEARCH_RESULTS[@]}"
  do
    ${DOCXBOX_BINARY} lsl "${PATH_DOCX}" --json fonts | grep -c "${i}"
    check_for_valgrind_error
  done
}

@test "${BATS_TEST_NUMBER}: \"docxbox ls filename.docx -lj searchString\" lists all files containing given searchString" {
  for i in "${SEARCH_RESULTS[@]}"
  do
    ${DOCXBOX_BINARY} ls "${PATH_DOCX}" -lj fonts | grep -c "${i}"
    check_for_valgrind_error
  done
}

@test "${BATS_TEST_NUMBER}: \"docxbox ls filename.docx --locate -j searchString\" lists all files containing given searchString" {
  for i in "${SEARCH_RESULTS[@]}"
  do
    ${DOCXBOX_BINARY} ls "${PATH_DOCX}" --locate -j fonts | grep -c "${i}"
    check_for_valgrind_error
  done
}

@test "${BATS_TEST_NUMBER}: \"docxbox ls filename.docx --locate --json searchString\" lists all files containing given searchString" {
  for i in "${SEARCH_RESULTS[@]}"
  do
    ${DOCXBOX_BINARY} ls "${PATH_DOCX}" --locate --json fonts | grep -c "${i}"
    check_for_valgrind_error
  done
}

@test "${BATS_TEST_NUMBER}: \"${CMD} nonexistent.docx searchString\" prints an error message" {
  run ${DOCXBOX_BINARY} lslj nonexistent.docx fonts
  [ "$status" -ne 0 ]

  check_for_valgrind_error

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
    check_for_valgrind_error
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
  local amount_opening=$(${DOCXBOX_BINARY} lslj "${PATH_DOCX}" fonts | grep --count "\[")
  local amount_closing=$(${DOCXBOX_BINARY} lslj "${PATH_DOCX}" fonts | grep --count "\]")

  (( amount_opening = amount_closing ))
}

@test "${BATS_TEST_NUMBER}: Amount opening and closing brackets \"{}\" must match" {
  local amount_opening=$(${DOCXBOX_BINARY} lslj "${PATH_DOCX}" fonts | grep --count "\{")
  local amount_closing=$(${DOCXBOX_BINARY} lslj "${PATH_DOCX}" fonts | grep --count "\}")

  (( amount_opening = amount_closing ))
}
