#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

#@todo: extend test
# rmt at the beginning
# rmt at the end
# rmt within

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
PATH_DOCX_NEW="test/tmp/cp_plain_text.docx"
PATH_DOCX_STYLES="test/tmp/cp_text_with_styles.docx"
ERR_LOG="test/tmp/err.log"

@test "Output of \"docxbox rmt {missing filename}\" is an error message" {
  run ${DOCXBOX_BINARY} rmt
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: DOCX filename" = "${lines[0]}" ]

  check_for_valgrind_error
}

title="Output of \"docxbox rmt filename.docx {missing arguments}\" "
title+="is an error message"
@test "${title}" {
  pattern="docxBox Error - Missing argument: \
String left-hand-side of part to be removed"

  run ${DOCXBOX_BINARY} rmt "${PATH_DOCX}"
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]

  check_for_valgrind_error
}

title_missing_argument="Output of \"docxbox rmt filename.docx leftHandString \
{missing_argument}\" is an error message"
@test "${title_missing_argument}" {
  pattern="docxBox Error - Missing argument: \
String right-hand-side of part to be removed"

  run ${DOCXBOX_BINARY} rmt "${PATH_DOCX}" "FooBar"
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]

  check_for_valgrind_error
}

title_base_functionality="With \"docxbox rmt filename.docx leftHandString \
rightHandString\" removes text between and including given strings"
@test "${title_base_functionality}" {
  pattern="Fugiat excepteursed in qui sit velit duis veniam."

  ${DOCXBOX_BINARY} lsl "${PATH_DOCX}" ${pattern} | grep --count "word/document.xml"

  run ${DOCXBOX_BINARY} rmt "${PATH_DOCX}" "Fugiat" "."
  [ "$status" -eq 0 ]

  check_for_valgrind_error

  ${DOCXBOX_BINARY} txt "${PATH_DOCX}" | grep --count --invert-match "${pattern}"
}

@test "Removing strings at the beginning of a file" {
  ${DOCXBOX_BINARY} rmt "${PATH_DOCX_NEW}" "THIS" "TITLE"

  check_for_valgrind_error

  ${DOCXBOX_BINARY} txt "${PATH_DOCX_NEW}" | grep --count "IN ALL CAPS"
}

@test "Removing strings at the beginning of a file within a sentence" {
  ${DOCXBOX_BINARY} rmt "${PATH_DOCX_NEW}" "TITLE" "ALL"

  check_for_valgrind_error

  ${DOCXBOX_BINARY} txt "${PATH_DOCX_NEW}" | grep --count "THIS IS A  CAPS"
}

@test "Removing strings in the middle of a file" {
  pattern="Text in cursive"

  ${DOCXBOX_BINARY} rmt "${PATH_DOCX_NEW}" "style" "cursive"

  check_for_valgrind_error

  ${DOCXBOX_BINARY} txt "${PATH_DOCX_NEW}" | grep --count --invert-match "${pattern}"
}

@test "Removing strings in the middle of a file within a sentence" {
  pattern="A style"

  ${DOCXBOX_BINARY} rmt "${PATH_DOCX_NEW}" " paragraph" "special"

  check_for_valgrind_error

  ${DOCXBOX_BINARY} txt "${PATH_DOCX_NEW}" | grep --count "${pattern}"
}

@test "Removing strings with different styles" {
  ${DOCXBOX_BINARY} rmt "${PATH_DOCX_NEW}" "CAPS" "paragraph"

  check_for_valgrind_error

  ${DOCXBOX_BINARY} txt "${PATH_DOCX_NEW}" | grep --count "without special style"
}

@test "Removing strings at the end of a file" {
  pattern="Bold text passages are great"

  ${DOCXBOX_BINARY} rmt "${PATH_DOCX_NEW}" "Bold" "great"

  check_for_valgrind_error

  ${DOCXBOX_BINARY} txt "${PATH_DOCX_NEW}" | grep --count --invert-match "${pattern}"
}

@test "Trying to remove strings at the end of a file with a nonexistent string" {
  pattern="Bold text passages are great"

  ${DOCXBOX_BINARY} rmt "${PATH_DOCX_NEW}" "Bold" "Foo"

  check_for_valgrind_error

  ${DOCXBOX_BINARY} txt "${PATH_DOCX_NEW}" | grep --count "${pattern}"
}

@test "Removing content between two given strings removes everything" {
  ${DOCXBOX_BINARY} lsi "${PATH_DOCX_STYLES}" | grep --count "image1.png"

  run ${DOCXBOX_BINARY} rmt "${PATH_DOCX_STYLES}" "FROM" "Until"
  [ "$status" -eq 0 ]

  check_for_valgrind_error

  ${DOCXBOX_BINARY} txt "${PATH_DOCX_STYLES}" | grep --count "I’m a dummy text file"
  ${DOCXBOX_BINARY} txt "${PATH_DOCX_STYLES}" | grep --count "next time also in BOLD"
  ${DOCXBOX_BINARY} txt "${PATH_DOCX_STYLES}" | grep --count --invert-match "I’m bold AND italic"

  ${DOCXBOX_BINARY} lsi "${PATH_DOCX_STYLES}" | grep --invert-match "image1.png"
}

@test "Output of \"docxbox rmt nonexistent.docx\" is an error message" {
  run ${DOCXBOX_BINARY} rmt nonexistent.docx
  [ "$status" -ne 0 ]

  ${DOCXBOX_BINARY} rmt nonexistent.docx Dolore incididunt 2>&1 | tee "${ERR_LOG}"
  check_for_valgrind_error
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
    check_for_valgrind_error
    cat "${ERR_LOG}" | grep --count "${pattern}"
  done
}

check_for_valgrind_error() {
  if $IS_VALGRIND_TEST; then
    cat "${VALGRIND_LOG}" | grep --count --invert-match "${VALGRIND_ERR_PATTERN}"
  fi
}
