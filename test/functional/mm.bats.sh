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

path_docx="test/tmp/cp_bio_assay.docx"

base_command="docxbox mm filename.docx"

@test "Output of \"docxbox mm {missing argument}\" is an error message" {
  run ${DOCXBOX_BINARY} mm
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: DOCX filename" = "${lines[0]}" ]
}

@test "Output of \"${base_command} {missing argument}\" is an error message" {
  pattern="docxBox Error - Missing argument: Meta attribute to be set"

  run ${DOCXBOX_BINARY} mm "${path_docx}"
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]
}

title="the meta attribute \"title\" can be modified"
@test "With \"${base_command} title {argument}\" ${title}" {
  run ${DOCXBOX_BINARY} mm "${path_docx}" title "someTitle"
  [ "$status" -eq 0 ]
  ${DOCXBOX_BINARY} lsm "${path_docx}" | grep --count "title: someTitle"
}

creator="the meta attribute \"creator\" can be modified"
@test "With \"${base_command} creator {argument}\" ${creator}" {
  run ${DOCXBOX_BINARY} mm "${path_docx}" creator "John Doe"
  [ "$status" -eq 0 ]
  ${DOCXBOX_BINARY} lsm "${path_docx}" | grep --count "creator: John Doe"
}

last_modified_by="the meta attribute \"lastModifiedBy\" can be modified"
@test "With \"${base_command} lastModifiedBy {argument}\" ${last_modified_by}" {
  pattern="lastModifiedBy: John Doe"

  run ${DOCXBOX_BINARY} mm "${path_docx}" lastModifiedBy "John Doe"
  [ "$status" -eq 0 ]
  ${DOCXBOX_BINARY} lsm "${path_docx}" | grep --count "${pattern}"
}

last_printed="the meta attribute \"lastPrinted\" can be modified"
@test "With \"${base_command} lastPrinted {argument}\" ${last_printed}" {
  print_date="2020-02-20T10:31:00Z"
  pattern="lastPrinted: 2020-02-20T10:31:00Z"

  run ${DOCXBOX_BINARY} mm "${path_docx}" lastPrinted $print_date
  [ "$status" -eq 0 ]
  ${DOCXBOX_BINARY} lsm "${path_docx}" | grep --count "${pattern}"
}

language="the meta attribute \"language\" can be modified"
@test "With \"${base_command} language {argument}\" ${language}" {
  run ${DOCXBOX_BINARY} mm "${path_docx}" language "de-CH"
  [ "$status" -eq 0 ]
  ${DOCXBOX_BINARY} lsm "${path_docx}" | grep --count "language: de-CH"
}

created="the meta attribute created can be changed"
created="the meta attribute \"created\" can be modified"
@test "With \"${base_command} created {argument}\" ${created}" {
  pattern="created: 2020-10-20T10:20:00Z"

  run ${DOCXBOX_BINARY} mm "${path_docx}" created "2020-10-20T10:20:00Z"
  [ "$status" -eq 0 ]
  ${DOCXBOX_BINARY} lsm "${path_docx}" | grep --count "${pattern}"
}

modified="the meta attribute \"modified\" can be modified"
@test "With \"${base_command} modified {argument}\" ${modified}" {
  pattern="modified: 2020-10-20T10:20:00Z"

  run ${DOCXBOX_BINARY} mm "${path_docx}" modified "2020-10-20T10:20:00Z"
  [ "$status" -eq 0 ]
  ${DOCXBOX_BINARY} lsm "${path_docx}" | grep --count "${pattern}"
}

@test "Modifying the meta attribute \"created\" does not change the meta attribute \"modified\"" {
  created=$(${DOCXBOX_BINARY} lsm "${path_docx}" | grep "created")

  run ${DOCXBOX_BINARY} mm "${path_docx}" modified "2020-10-20T10:20:00Z"

  ${DOCXBOX_BINARY} lsm "${path_docx}" | grep "${created}"
}

revision="the meta attribute \"revision\" can be changed"
@test "With \"${base_command} revision {argument}\" ${revision}" {
  run ${DOCXBOX_BINARY} mm "${path_docx}" revision "25"
  [ "$status" -eq 0 ]
  ${DOCXBOX_BINARY} lsm "${path_docx}" | grep "revision: 25"
}


@test "Output of \"docxbox mm nonexistent.docx\" is an error message" {
  err_log="test/tmp/err.log"

  run ${DOCXBOX_BINARY} mm nonexistent.docx
  [ "$status" -ne 0 ]

  ${DOCXBOX_BINARY} mm nonexistent.docx revision "1" 2>&1 | tee "${err_log}"
  cat "${err_log}" | grep --count "docxBox Error - File not found:"
}

argument="{meta_attribute} {argument}"
@test "Output of \"docxbox mm ${argument} wrong_file_type\" is an error message" {
  pattern="docxBox Error - File is no ZIP archive:"
  err_log="test/tmp/err.log"
  wrong_file_types=(
  "test/tmp/cp_lorem_ipsum.pdf"
  "test/tmp/cp_mock_csv.csv"
  "test/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    ${DOCXBOX_BINARY} lorem "${i}" revision "12" 2>&1 | tee "${err_log}"
    cat "${err_log}" | grep --count "${pattern}"
  done
}
