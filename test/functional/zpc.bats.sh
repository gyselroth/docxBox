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
UNZIPPED_DOCX_DIRECTORY="cp_table_unordered_list_images.docx-extracted"

@test "Case 1: Output of \"docxbox zpc {missing argument}\" is an error message" {
  pattern="docxBox Error - Missing argument: Path of directory to be zipped"

  run ${DOCXBOX_BINARY} zpc
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]

  check_for_valgrind_error
}

@test "Case 2: Output of \"docxbox zpc directory {missing argument}\" is an error message" {
  pattern="docxBox Error - Missing argument: Filename of docx to be created"

  run ${DOCXBOX_BINARY} zpc "${UNZIPPED_DOCX_DIRECTORY}"
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]

  check_for_valgrind_error
}

title="Case 3: With \"docxbox zp directory /path-to-file/filename.docx\" "
title+="a directory can be zipped into a docx"
@test "$title" {
  if [ ! -d "${UNZIPPED_DOCX_DIRECTORY}" ]; then
    ${DOCXBOX_BINARY} uzi "${PATH_DOCX}"
  fi

  path_new_docx="test/tmp/zp_table_unordered_list_images.docx"

  run ${DOCXBOX_BINARY} zpc "${UNZIPPED_DOCX_DIRECTORY}" "${path_new_docx}"
  [ "$status" -eq 0 ]

  check_for_valgrind_error

  ls test/tmp | grep -c zp_table_unordered_list_images.docx

  if [ -d "${UNZIPPED_DOCX_DIRECTORY}" ]; then
    rm --recursive "${UNZIPPED_DOCX_DIRECTORY}"
  fi
}

check_for_valgrind_error() {
  if $IS_VALGRIND_TEST; then
    cat "${VALGRIND_LOG}" | grep --count --invert-match "${VALGRIND_ERR_PATTERN}"
  fi
}
