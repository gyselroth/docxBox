#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

VALGRIND="valgrind -v --leak-check=full\
 --log-file=test/assets/documents/other/mem-leak.log"

if $IS_VALGRIND_TEST;
then
  DOCXBOX_BINARY="${VALGRIND} $BATS_TEST_DIRNAME/../tmp/docxbox"
else
  DOCXBOX_BINARY="$BATS_TEST_DIRNAME/../tmp/docxbox"
fi

PATH_DOCX="test/tmp/cp_bio_assay.docx"
ERR_LOG="test/tmp/err.log"

BASE_COMMAND="docxbox mm filename.docx"

@test "Output of \"docxbox mm {missing argument}\" is an error message" {
  run ${DOCXBOX_BINARY} mm
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: DOCX filename" = "${lines[0]}" ]
}

@test "Output of \"${BASE_COMMAND} {missing argument}\" is an error message" {
  pattern="docxBox Error - Missing argument: Meta attribute to be set"

  run ${DOCXBOX_BINARY} mm "${PATH_DOCX}"
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]
}

title="the meta attribute \"title\" can be modified"
@test "With \"${BASE_COMMAND} title {argument}\" ${title}" {
  run ${DOCXBOX_BINARY} mm "${PATH_DOCX}" title "someTitle"
  [ "$status" -eq 0 ]
  ${DOCXBOX_BINARY} lsm "${PATH_DOCX}" | grep --count "title: someTitle"
}

creator="the meta attribute \"creator\" can be modified"
@test "With \"${BASE_COMMAND} creator {argument}\" ${creator}" {
  run ${DOCXBOX_BINARY} mm "${PATH_DOCX}" creator "John Doe"
  [ "$status" -eq 0 ]
  ${DOCXBOX_BINARY} lsm "${PATH_DOCX}" | grep --count "creator: John Doe"
}

last_modified_by="the meta attribute \"lastModifiedBy\" can be modified"
@test "With \"${BASE_COMMAND} lastModifiedBy {argument}\" ${last_modified_by}" {
  pattern="lastModifiedBy: John Doe"

  run ${DOCXBOX_BINARY} mm "${PATH_DOCX}" lastModifiedBy "John Doe"
  [ "$status" -eq 0 ]
  ${DOCXBOX_BINARY} lsm "${PATH_DOCX}" | grep --count "${pattern}"
}

last_printed="the meta attribute \"lastPrinted\" can be modified"
@test "With \"${BASE_COMMAND} lastPrinted {argument}\" ${last_printed}" {
  print_date="2020-02-20T10:31:00Z"
  pattern="lastPrinted: 2020-02-20T10:31:00Z"

  run ${DOCXBOX_BINARY} mm "${PATH_DOCX}" lastPrinted $print_date
  [ "$status" -eq 0 ]
  ${DOCXBOX_BINARY} lsm "${PATH_DOCX}" | grep --count "${pattern}"
}

language="the meta attribute \"language\" can be modified"
@test "With \"${BASE_COMMAND} language {argument}\" ${language}" {
  run ${DOCXBOX_BINARY} mm "${PATH_DOCX}" language "de-CH"
  [ "$status" -eq 0 ]
  ${DOCXBOX_BINARY} lsm "${PATH_DOCX}" | grep --count "language: de-CH"
}

created="the meta attribute \"created\" can be modified"
@test "With \"${BASE_COMMAND} created {argument}\" ${created}" {
  pattern="created: 2020-10-20T10:20:00Z"

  run ${DOCXBOX_BINARY} mm "${PATH_DOCX}" created "2020-10-20T10:20:00Z"
  [ "$status" -eq 0 ]
  ${DOCXBOX_BINARY} lsm "${PATH_DOCX}" | grep --count "${pattern}"
}

modified="the meta attribute \"modified\" can be modified"
@test "With \"${BASE_COMMAND} modified {argument}\" ${modified}" {
  pattern="modified: 2020-10-20T10:20:00Z"

  run ${DOCXBOX_BINARY} mm "${PATH_DOCX}" modified "2020-10-20T10:20:00Z"
  [ "$status" -eq 0 ]
  ${DOCXBOX_BINARY} lsm "${PATH_DOCX}" | grep --count "${pattern}"
}

@test "Modifying the meta attribute \"created\" does not change the meta attribute \"modified\"" {
  created=$(${DOCXBOX_BINARY} lsm "${PATH_DOCX}" | grep "created")

  run ${DOCXBOX_BINARY} mm "${PATH_DOCX}" modified "2020-10-20T10:20:00Z"

  ${DOCXBOX_BINARY} lsm "${PATH_DOCX}" | grep "${created}"
}

revision="the meta attribute \"revision\" can be changed"
@test "With \"${BASE_COMMAND} revision {argument}\" ${revision}" {
  run ${DOCXBOX_BINARY} mm "${PATH_DOCX}" revision "25"
  [ "$status" -eq 0 ]
  ${DOCXBOX_BINARY} lsm "${PATH_DOCX}" | grep "revision: 25"
}


@test "Output of \"docxbox mm nonexistent.docx\" is an error message" {
  run ${DOCXBOX_BINARY} mm nonexistent.docx
  [ "$status" -ne 0 ]

  ${DOCXBOX_BINARY} mm nonexistent.docx revision "1" 2>&1 | tee "${ERR_LOG}"
  cat "${ERR_LOG}" | grep --count "docxBox Error - File not found:"
}

argument="{meta_attribute} {argument}"
@test "Output of \"docxbox mm ${argument} wrong_file_type\" is an error message" {
  pattern="docxBox Error - File is no ZIP archive:"
  wrong_file_types=(
  "test/tmp/cp_lorem_ipsum.pdf"
  "test/tmp/cp_mock_csv.csv"
  "test/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    ${DOCXBOX_BINARY} lorem "${i}" revision "12" 2>&1 | tee "${ERR_LOG}"
    cat "${ERR_LOG}" | grep --count "${pattern}"
  done
}
