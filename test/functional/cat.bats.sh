#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

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
PATH_XML="word/document.xml"

@test "Case 1: Output of \"docxbox cat {missing argument}\" is an error message" {
  run ${DOCXBOX_BINARY} cat
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: DOCX filename" = "${lines[0]}" ]

  check_for_valgrind_error
}

@test "Case 2: Output of \"docxbox cat filename.docx {missing argument}\" is an error message" {
  run ${DOCXBOX_BINARY} cat "${PATH_DOCX}"
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: File to be displayed" = "${lines[0]}" ]

  check_for_valgrind_error
}

@test "Case 3: With \"docxbox cat filename.docx filename.xml\" the XML is displayed" {
  ${DOCXBOX_BINARY} cat "${PATH_DOCX}" "${PATH_XML}" | grep "^[[:space:]]\{4\}"

  check_for_valgrind_error
}

check_for_valgrind_error() {
  if $IS_VALGRIND_TEST; then
    cat "${VALGRIND_LOG}" | grep --count --invert-match "${VALGRIND_ERR_PATTERN}"
  fi
}
