#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

VALGRIND="valgrind -v --leak-check=full\
 --log-file=test/tmp/mem-leak.log"

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

@test "Exit code of ${BASE_COMMAND} filename.docx\" is zero" {
  run ${DOCXBOX_BINARY} lsd "${PATH_DOCX}"
  [ "$status" -eq 0 ]
}

@test "Output of ${BASE_COMMAND} {missing argument}\" is an error message" {
  run ${DOCXBOX_BINARY} lsd
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: Filename of DOCX to be extracted" = "${lines[0]}" ]
}

@test "With ${BASE_COMMAND} filename.docx\" the fields in the docx are listed" {
  ${DOCXBOX_BINARY} lsd "${PATH_DOCX}" | grep --count "${MERGE_FIELD}"
  ${DOCXBOX_BINARY} lsd "${PATH_DOCX}" | grep --count "${MERGE_FORMAT}"
}

@test "With ${BASE_COMMAND} filename.docx\" the fields in the header are listed" {
  ${DOCXBOX_BINARY} lsd "${PATH_DOCX}" | grep --count "MERGEFIELD  Mergefield_Header"
}

@test "With ${BASE_COMMAND} filename.docx\" the fields in the footer are listed" {
  ${DOCXBOX_BINARY} lsd "${PATH_DOCX}" | grep --count "MERGEFIELD  Mergefield_Footer"
}

title="With \"docxbox ls filename.docx --fields\" "
title+="the fields in the docx are listed"
@test "$title" {
  ${DOCXBOX_BINARY} lsd "${PATH_DOCX}" | grep --count "${MERGE_FIELD}"
  ${DOCXBOX_BINARY} lsd "${PATH_DOCX}" | grep --count "${MERGE_FORMAT}"
}

@test "With \"docxbox ls filename.docx -d\" the fields in the docx are listed" {
  ${DOCXBOX_BINARY} ls "${PATH_DOCX}" -d | grep --count "${MERGE_FIELD}"
  ${DOCXBOX_BINARY} ls "${PATH_DOCX}" -d | grep --count "${MERGE_FORMAT}"
}

@test "Output of \"docxbox lsd nonexistent.docx\" is an error message" {
  run ${DOCXBOX_BINARY} lsd nonexistent.docx
  [ "$status" -ne 0 ]

  ${DOCXBOX_BINARY} lsd nonexistent.docx 2>&1 | tee "${ERR_LOG}"
  cat "${ERR_LOG}" | grep --count "docxBox Error - File not found:"
}

@test "Output of ${BASE_COMMAND} wrong_file_type\" is an error message" {
  wrong_file_types=(
  "test/tmp/cp_lorem_ipsum.pdf"
  "test/tmp/cp_mock_csv.csv"
  "test/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    ${DOCXBOX_BINARY} lsd "${i}" 2>&1 | tee "${ERR_LOG}"
    cat "${ERR_LOG}" | grep --count "docxBox Error - File is no ZIP archive:"
  done
}
