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

BASE_COMMAND="\"docxbox lsd"

PATH_DOCX="test/tmp/cp_mergefields.docx"
ERR_LOG="test/tmp/err.log"

MERGE_FIELD="MERGEFIELD"
MERGE_FORMAT="\* MERGEFORMAT"

@test "Case 1: Exit code of ${BASE_COMMAND} filename.docx\" is zero" {
  run ${DOCXBOX_BINARY} lsd "${PATH_DOCX}"
  [ "$status" -eq 0 ]

  check_for_valgrind_error
}

@test "Case 2: Output of ${BASE_COMMAND} {missing argument}\" is an error message" {
  run ${DOCXBOX_BINARY} lsd
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: Filename of DOCX to be extracted" = "${lines[0]}" ]

  check_for_valgrind_error
}

@test "Case 3: With ${BASE_COMMAND} filename.docx\" the fields in the docx are listed" {
  ${DOCXBOX_BINARY} lsd "${PATH_DOCX}" | grep --count "${MERGE_FIELD}"
  ${DOCXBOX_BINARY} lsd "${PATH_DOCX}" | grep --count "${MERGE_FORMAT}"

  check_for_valgrind_error
}

@test "Case 4: With ${BASE_COMMAND} filename.docx\" the fields in the header are listed" {
  ${DOCXBOX_BINARY} lsd "${PATH_DOCX}" | grep --count "MERGEFIELD  Mergefield_Header"

  check_for_valgrind_error
}

@test "Case 5: With ${BASE_COMMAND} filename.docx\" the fields in the footer are listed" {
  ${DOCXBOX_BINARY} lsd "${PATH_DOCX}" | grep --count "MERGEFIELD  Mergefield_Footer"

  check_for_valgrind_error
}

title="Case 6: With \"docxbox ls filename.docx --fields\" "
title+="the fields in the docx are listed"
@test "$title" {
  ${DOCXBOX_BINARY} lsd "${PATH_DOCX}" | grep --count "${MERGE_FIELD}"
  ${DOCXBOX_BINARY} lsd "${PATH_DOCX}" | grep --count "${MERGE_FORMAT}"

  check_for_valgrind_error
}

@test "Case 7: With \"docxbox ls filename.docx -d\" the fields in the docx are listed" {
  ${DOCXBOX_BINARY} ls "${PATH_DOCX}" -d | grep --count "${MERGE_FIELD}"
  ${DOCXBOX_BINARY} ls "${PATH_DOCX}" -d | grep --count "${MERGE_FORMAT}"

  check_for_valgrind_error
}

@test "Case 8: Output of \"docxbox lsd nonexistent.docx\" is an error message" {
  run ${DOCXBOX_BINARY} lsd nonexistent.docx
  [ "$status" -ne 0 ]

  check_for_valgrind_error

  ${DOCXBOX_BINARY} lsd nonexistent.docx 2>&1 | tee "${ERR_LOG}"
  cat "${ERR_LOG}" | grep --count "docxBox Error - File not found:"
}

@test "Case 9: Output of ${BASE_COMMAND} wrong_file_type\" is an error message" {
  wrong_file_types=(
  "test/tmp/cp_lorem_ipsum.pdf"
  "test/tmp/cp_mock_csv.csv"
  "test/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    ${DOCXBOX_BINARY} lsd "${i}" 2>&1 | tee "${ERR_LOG}"
    check_for_valgrind_error
    cat "${ERR_LOG}" | grep --count "docxBox Error - File is no ZIP archive:"
  done
}

check_for_valgrind_error() {
  if $IS_VALGRIND_TEST; then
    cat "${VALGRIND_LOG}" | grep --count --invert-match "${VALGRIND_ERR_PATTERN}"
  fi
}
