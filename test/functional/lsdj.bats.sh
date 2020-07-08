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

BASE_COMMAND="docxbox lsdj filename.docx"

PATH_DOCX="test/tmp/cp_table_unordered_list_images.docx"
ERR_LOG="test/tmp/err.log"

LONG_DESCRIPTION_JSON="the fields in the docx are listed as JSON"

@test "Case 1: Exit code of \"${BASE_COMMAND}\" is zero" {
  run ${DOCXBOX_BINARY} lsdj "${PATH_DOCX}"
  [ "$status" -eq 0 ]

  check_for_valgrind_error
}

@test "Case 2: Output of \"docxbox lsdj {missing argument}\" is an error message" {
  run ${DOCXBOX_BINARY} lsdj
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: Filename of DOCX to be extracted" = "${lines[0]}" ]

  check_for_valgrind_error
}

@test "Case 3: With \"${BASE_COMMAND}\" the fields in the docx are listed as JSON" {
  pattern="table_unordered_list_images.docx-"
  ${DOCXBOX_BINARY} lsdj "${PATH_DOCX}" | grep --count "${pattern}"

  check_for_valgrind_error

  pattern="/word/document.xml"
  ${DOCXBOX_BINARY} lsdj "${PATH_DOCX}" | grep --count "${pattern}"

  check_for_valgrind_error
}

longhand="--fields --json"
title="Case 4: With \"docxbox ls filename.docx ${longhand}\" "
title+="${LONG_DESCRIPTION_JSON}"
@test "${title}" {
  pattern="table_unordered_list_images.docx-"
  ${DOCXBOX_BINARY} ls "${PATH_DOCX}" ${longhand} | grep --count "${pattern}"

  check_for_valgrind_error

  pattern="/word/document.xml"
  ${DOCXBOX_BINARY} ls "${PATH_DOCX}" ${longhand} | grep --count "${pattern}"

  check_for_valgrind_error
}

@test "Case 5: With \"docxbox ls filename.docx -dj\" ${LONG_DESCRIPTION_JSON}" {
  pattern="table_unordered_list_images.docx-"
  ${DOCXBOX_BINARY} ls "${PATH_DOCX}" -dj | grep --count "${pattern}"

  pattern="/word/document.xml"
  ${DOCXBOX_BINARY} ls "${PATH_DOCX}" -dj | grep --count "${pattern}"

  check_for_valgrind_error
}

@test "Case 6: With \"docxbox lsd filename.docx --json\" ${LONG_DESCRIPTION_JSON}" {
  pattern="table_unordered_list_images.docx-"
  ${DOCXBOX_BINARY} lsd "${PATH_DOCX}" --json | grep --count "${pattern}"

  check_for_valgrind_error

  pattern="/word/document.xml"
  ${DOCXBOX_BINARY} lsd "${PATH_DOCX}" --json | grep --count "${pattern}"

  check_for_valgrind_error
}

@test "Case 7: Output of \"docxbox lsdj nonexistent.docx\" is an error message" {
  run ${DOCXBOX_BINARY} lsdj nonexistent.docx
  [ "$status" -ne 0 ]
  check_for_valgrind_error

  ${DOCXBOX_BINARY} lsdj nonexistent.docx 2>&1 | tee "${ERR_LOG}"
  cat "${ERR_LOG}" | grep --count "docxBox Error - File not found:"
}

@test "Case 8: Output of \"${BASE_COMMAND} wrong_file_type\" is an error message" {
  wrong_file_types=(
  "test/tmp/cp_lorem_ipsum.pdf"
  "test/tmp/cp_mock_csv.csv"
  "test/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    ${DOCXBOX_BINARY} lsdj "${i}" 2>&1 | tee "${ERR_LOG}"
    check_for_valgrind_error
    cat "${ERR_LOG}" | grep --count "docxBox Error - File is no ZIP archive:"
  done
}

check_for_valgrind_error() {
  if $IS_VALGRIND_TEST; then
    cat "${VALGRIND_LOG}" | grep --count --invert-match "${VALGRIND_ERR_PATTERN}"
  fi
}
