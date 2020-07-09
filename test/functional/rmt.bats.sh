#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

CMD="docxbox rmt"  # current command

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
PATH_DOCX_NEW="test/tmp/cp_plain_text.docx"
PATH_DOCX_STYLES="test/tmp/cp_text_with_styles.docx"
ERR_LOG="test/tmp/err.log"

@test "$BATS_TEST_NUMBER: \"$CMD {no filename}\" prints an error message" {
  run ${DOCXBOX_BINARY} rmt
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: DOCX filename" = "${lines[0]}" ]

  check_for_valgrind_error
}

@test "$BATS_TEST_NUMBER: \"$CMD filename.docx {no arguments}\" prints an error message" {
  local pattern="docxBox Error - Missing argument: \
String left-hand-side of part to be removed"

  run ${DOCXBOX_BINARY} rmt "${PATH_DOCX}"
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]

  check_for_valgrind_error
}

@test "$BATS_TEST_NUMBER: \"$CMD filename.docx leftHandString {no arguments}\" prints an error message" {
  local pattern="docxBox Error - Missing argument: \
String right-hand-side of part to be removed"

  run ${DOCXBOX_BINARY} rmt "${PATH_DOCX}" "FooBar"
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]

  check_for_valgrind_error
}

@test "$BATS_TEST_NUMBER: \"$CMD filename.docx leftHandString rightHandString\" removes text between and including given strings" {
  pattern="Fugiat excepteursed in qui sit velit duis veniam."

  ${DOCXBOX_BINARY} lsl "${PATH_DOCX}" ${pattern} | grep --count "word/document.xml"

  run ${DOCXBOX_BINARY} rmt "${PATH_DOCX}" "Fugiat" "."
  [ "$status" -eq 0 ]

  check_for_valgrind_error

  ${DOCXBOX_BINARY} txt "${PATH_DOCX}" | grep --count --invert-match "${pattern}"
}


@test "$BATS_TEST_NUMBER: Removing strings at the beginning of a file" {
  ${DOCXBOX_BINARY} rmt "${PATH_DOCX_NEW}" "THIS" "TITLE"

  check_for_valgrind_error

  ${DOCXBOX_BINARY} txt "${PATH_DOCX_NEW}" | grep --count "IN ALL CAPS"
}

@test "$BATS_TEST_NUMBER: Removing strings at the beginning of a file within a sentence" {
  ${DOCXBOX_BINARY} rmt "${PATH_DOCX_NEW}" "TITLE" "ALL"

  check_for_valgrind_error

  ${DOCXBOX_BINARY} txt "${PATH_DOCX_NEW}" | grep --count "THIS IS A  CAPS"
}

@test "$BATS_TEST_NUMBER: Removing strings in the middle of a file" {
  local pattern="Text in cursive"

  ${DOCXBOX_BINARY} rmt "${PATH_DOCX_NEW}" "style" "cursive"

  check_for_valgrind_error

  ${DOCXBOX_BINARY} txt "${PATH_DOCX_NEW}" | grep --count --invert-match "${pattern}"
}

@test "$BATS_TEST_NUMBER: Removing strings in the middle of a file within a sentence" {
  local pattern="A style"

  ${DOCXBOX_BINARY} rmt "${PATH_DOCX_NEW}" " paragraph" "special"

  check_for_valgrind_error

  ${DOCXBOX_BINARY} txt "${PATH_DOCX_NEW}" | grep --count "${pattern}"
}

@test "$BATS_TEST_NUMBER: Removing strings w/ different styles" {
  ${DOCXBOX_BINARY} rmt "${PATH_DOCX_NEW}" "CAPS" "paragraph"

  check_for_valgrind_error

  ${DOCXBOX_BINARY} txt "${PATH_DOCX_NEW}" | grep --count "without special style"
}

@test "$BATS_TEST_NUMBER: Removing strings at the end of a file" {
  local pattern="Bold text passages are great"

  local before_rmt
  before_rmt=$(${DOCXBOX_BINARY} txt "${PATH_DOCX_NEW}" | wc --words)

  ${DOCXBOX_BINARY} rmt "${PATH_DOCX_NEW}" "Bold" "great"

  check_for_valgrind_error

  after_rmt=$(${DOCXBOX_BINARY} txt "${PATH_DOCX_NEW}" | wc --words)

  (( before_rmt = after_rmt ))
}

@test "$BATS_TEST_NUMBER: Trying to remove strings at the end of a file w/ a nonexistent string" {
  local pattern="Bold text passages are great"

  ${DOCXBOX_BINARY} rmt "${PATH_DOCX_NEW}" "Bold" "Foo"

  check_for_valgrind_error

  ${DOCXBOX_BINARY} txt "${PATH_DOCX_NEW}" | grep --count "${pattern}"
}

@test "$BATS_TEST_NUMBER: Removing content between two given strings removes everything" {
  ${DOCXBOX_BINARY} lsi "${PATH_DOCX_STYLES}" | grep --count "image1.png"

  run ${DOCXBOX_BINARY} rmt "${PATH_DOCX_STYLES}" "FROM" "Until"
  [ "$status" -eq 0 ]

  check_for_valgrind_error

  ${DOCXBOX_BINARY} txt "${PATH_DOCX_STYLES}" | grep --count "I’m a dummy text file"
  ${DOCXBOX_BINARY} txt "${PATH_DOCX_STYLES}" | grep --count "next time also in BOLD"
  ${DOCXBOX_BINARY} txt "${PATH_DOCX_STYLES}" | grep --count --invert-match "I’m bold AND italic"

  ${DOCXBOX_BINARY} lsi "${PATH_DOCX_STYLES}" | grep --invert-match "image1.png"
}

@test "$BATS_TEST_NUMBER: \"${CMD} nonexistent.docx\" prints an error message" {
  run ${DOCXBOX_BINARY} rmt nonexistent.docx
  [ "$status" -ne 0 ]

  ${DOCXBOX_BINARY} rmt nonexistent.docx Dolore incididunt 2>&1 | tee "${ERR_LOG}"
  check_for_valgrind_error
  cat "${ERR_LOG}" | grep --count "docxBox Error - File not found:"
}

@test "$BATS_TEST_NUMBER: \"${CMD} wrong_file_type\" prints an error message" {
  local pattern="docxBox Error - File is no ZIP archive:"
  local wrong_file_types=(
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
