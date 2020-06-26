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

PATH_DOCX="test/tmp/cp_table_unordered_list_images.docx"
ERR_LOG="test/tmp/err.log"

@test "Output of \"docxbox rmt {missing filename}\" is an error message" {
  run ${DOCXBOX_BINARY} rmt
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: DOCX filename" = "${lines[0]}" ]
}

title="Output of \"docxbox rmt filename.docx {missing arguments}\" "
title+="is an error message"
@test "${title}" {
  pattern="docxBox Error - Missing argument: \
String left-hand-side of part to be removed"

  run ${DOCXBOX_BINARY} rmt "${PATH_DOCX}"
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]
}

@test "Output of \"docxbox rmt filename.docx leftHandString {missing_argument}\" is an error message" {
  pattern="docxBox Error - Missing argument: \
String right-hand-side of part to be removed"

  run ${DOCXBOX_BINARY} rmt "${PATH_DOCX}" "FooBar"
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]
}

@test "With \"docxbox rmt filename.docx leftHandString rightHandString\" removes text between and including given strings" {
  pattern="Fugiat excepteursed in qui sit velit duis veniam."

  ${DOCXBOX_BINARY} lsl "${PATH_DOCX}" ${pattern} | grep --count "word/document.xml"

  run ${DOCXBOX_BINARY} rmt "${PATH_DOCX}" "Fugiat" "."
  [ "$status" -eq 0 ]

  ${DOCXBOX_BINARY} txt "${PATH_DOCX}" | grep --count --invert-match "${pattern}"
}

@test "Output of \"docxbox rmt nonexistent.docx\" is an error message" {
  run ${DOCXBOX_BINARY} rmt nonexistent.docx
  [ "$status" -ne 0 ]

  ${DOCXBOX_BINARY} rmt nonexistent.docx Dolore incididunt 2>&1 | tee "${ERR_LOG}"
  cat "${ERR_LOG}" | grep --count "docxBox Error - File not found:"
}

@test "Output of \"docxbox rmt wrong_file_type\" is an error message" {
  pattern="docxBox Error - File is no ZIP archive:"
  wrong_file_types=(
  "test/tmp/cp_lorem_ipsum.pdf"
  "test/tmp/cp_mock_csv.csv"
  "test/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    ${DOCXBOX_BINARY} rmt "${i}" Dolore incididunt 2>&1 | tee "${ERR_LOG}"
    cat "${ERR_LOG}" | grep --count "${pattern}"
  done
}
