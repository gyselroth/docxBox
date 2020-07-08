#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

VALGRIND_LOG="test/tmp/mem-leak.log"
VALGRIND="valgrind -v --leak-check=full\
 --log-file=${VALGRIND_LOG}"

VALGRIND_ERR_PATTERN="ERROR SUMMARY: [0-9] errors from [0-9] contexts"

if $IS_VALGRIND_TEST;
then
  DOCXBOX_BINARY="${VALGRIND} $BATS_TEST_DIRNAME/../tmp/docxbox"
else
  DOCXBOX_BINARY="$BATS_TEST_DIRNAME/../tmp/docxbox"
fi

PATH_DOCX="test/tmp/cp_table_unordered_list_images.docx"
ERR_LOG="test/tmp/err.log"

BASE_COMMAND="docxbox lsmj filename.docx"

DESCRIPTION="contains information about the creation time and date"

PATTERN_CREATED="\"created\": \"2020-06-18T10:30:11Z\""

@test "Case 1: Exit code of \"${BASE_COMMAND}\" is zero" {
  run ${DOCXBOX_BINARY} lsmj "${PATH_DOCX}"
  [ "$status" -eq 0 ]

  check_for_valgrind_error
}

@test "Case 2: Output of \"docxbox lsmj {missing argument}\" is an error message" {
  pattern="docxBox Error - Missing argument: Filename of DOCX to be extracted"

  run ${DOCXBOX_BINARY} lsmj
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]

  check_for_valgrind_error
}

@test "Case 3: Output of \"${BASE_COMMAND}\" contains information about the xml schema" {
  pattern="\"xmlSchema\": \"http://schemas.openxmlformats.org/officeDocument/2006\""

  ${DOCXBOX_BINARY} lsmj "${PATH_DOCX}" | grep --count "${pattern}"

  check_for_valgrind_error
}

@test "Case 4: Output of \"docxbox lsm filename.docx --json\" ${DESCRIPTION}" {
  ${DOCXBOX_BINARY} lsm "${PATH_DOCX}" --json | grep --count "${PATTERN_CREATED}"

  check_for_valgrind_error
}

@test "Case 5: Output of \"docxbox lsm filename.docx -j\" ${DESCRIPTION}" {
  ${DOCXBOX_BINARY} lsm "${PATH_DOCX}" -j | grep --count "${PATTERN_CREATED}"

  check_for_valgrind_error
}

@test "Case 6: Output of \"docxbox ls filename.docx --meta --json\" ${DESCRIPTION}" {
  ${DOCXBOX_BINARY} ls "${PATH_DOCX}" --meta --json | grep --count "${PATTERN_CREATED}"

  check_for_valgrind_error
}

@test "Case 7: Output of \"docxbox ls filename.docx -mj\" ${DESCRIPTION}" {
  ${DOCXBOX_BINARY} ls "${PATH_DOCX}" -mj | grep --count "${PATTERN_CREATED}"

  check_for_valgrind_error
}

@test "Case 8: Output of \"${BASE_COMMAND}\" contains language information" {
  pattern="\"language\": \"en-US\""

  ${DOCXBOX_BINARY} lsmj "${PATH_DOCX}" | grep --count "${pattern}"

  check_for_valgrind_error
}

@test "Case 9: Output of \"${BASE_COMMAND}\" contains information about the revision" {
  ${DOCXBOX_BINARY} lsmj "${PATH_DOCX}" | grep --count "\"revision\": \"2\""

  check_for_valgrind_error
}

@test "Case 10: Output of \"docxbox lsmj nonexistent.docx\" is an error message" {
  run ${DOCXBOX_BINARY} lsmj nonexistent.docx
  [ "$status" -ne 0 ]

  check_for_valgrind_error

  ${DOCXBOX_BINARY} lsmj nonexistent.docx 2>&1 | tee "${ERR_LOG}"
  cat "${ERR_LOG}" | grep --count "docxBox Error - File not found:"
}

@test "Case 11: Output of \"docxbox lsmj wrong_file_type\" is an error message" {
  pattern="docxBox Error - File is no ZIP archive:"
  wrong_file_types=(
  "test/tmp/cp_lorem_ipsum.pdf"
  "test/tmp/cp_mock_csv.csv"
  "test/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    ${DOCXBOX_BINARY} lsmj "${i}" 2>&1 | tee "${ERR_LOG}"
    check_for_valgrind_error
    cat "${ERR_LOG}" | grep --count "${pattern}"
  done
}

check_for_valgrind_error() {
  if $IS_VALGRIND_TEST; then
    cat "${VALGRIND_LOG}" | grep --count --invert-match "${VALGRIND_ERR_PATTERN}"
  fi
}
