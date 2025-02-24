#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper
source ./test/functional/_set_docxbox_binary.sh

CMD="docxbox rmt"  # current command

PATH_DOCX="test/tmp/cp_table_unordered_list_images.docx"
PATH_DOCX_NEW="test/tmp/cp_plain_text.docx"
PATH_DOCX_STYLES="test/tmp/cp_text_with_styles.docx"
ERR_LOG="test/tmp/err.log"

@test "$BATS_TEST_NUMBER: \"$CMD {no filename}\" prints an error message" {
  run "${DOCXBOX_BINARY}" rmt
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: DOCX filename" = "${lines[0]}" ]

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "$BATS_TEST_NUMBER: \"$CMD filename.docx {no arguments}\" prints an error message" {
  local pattern="docxBox Error - Missing argument: \
String left-hand-side of part to be removed"

  run "${DOCXBOX_BINARY}" rmt "${PATH_DOCX}"
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "$BATS_TEST_NUMBER: \"$CMD filename.docx leftHandString {no arguments}\" prints an error message" {
  local pattern="docxBox Error - Missing argument: \
String right-hand-side of part to be removed"

  run "${DOCXBOX_BINARY}" rmt "${PATH_DOCX}" "FooBar"
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "$BATS_TEST_NUMBER: \"$CMD filename.docx leftHandString rightHandString\" removes text between and including given strings" {
  local before_rmt
# before_rmt=$(${DOCXBOX_BINARY} txt "${PATH_DOCX}" | grep --count "Fugiat")
  before_rmt=$(${DOCXBOX_BINARY} txt "${PATH_DOCX}" | grep --count "${pattern}")

  ${DOCXBOX_BINARY} lsl "${PATH_DOCX}" "${pattern}" | grep --count "word/document.xml"

  run "${DOCXBOX_BINARY}" rmt "${PATH_DOCX}" "Fugiat" "."
  [ "$status" -eq 0 ]

  source ./test/functional/_check_for_valgrind_errors.sh

  local after_rmt
  after_rmt=$(${DOCXBOX_BINARY} txt "${PATH_DOCX}" | grep --count "Fugiat")

  (( before_rmt > after_rmt ))
  (( after_rmt == "0" ))
}

@test "$BATS_TEST_NUMBER: Removing strings at the beginning of a file" {
  local before_rmt
  before_rmt=$(${DOCXBOX_BINARY} txt "${PATH_DOCX_NEW}" | grep --count "IS A")

  ${DOCXBOX_BINARY} rmt "${PATH_DOCX_NEW}" "THIS" "TITLE"

  source ./test/functional/_check_for_valgrind_errors.sh

  local after_rmt
  after_rmt=$(${DOCXBOX_BINARY} txt "${PATH_DOCX_NEW}" | grep --count "IS A")

  (( before_rmt > after_rmt ))

  ${DOCXBOX_BINARY} txt "${PATH_DOCX_NEW}" | grep --count "IN ALL CAPS"
}

@test "$BATS_TEST_NUMBER: Removing strings at the beginning of a file within a sentence" {
  ${DOCXBOX_BINARY} rmt "${PATH_DOCX_NEW}" "TITLE" "ALL"

  source ./test/functional/_check_for_valgrind_errors.sh

  ${DOCXBOX_BINARY} txt "${PATH_DOCX_NEW}" | grep --count "THIS IS A  CAPS"
}

@test "$BATS_TEST_NUMBER: Removing strings in the middle of a file" {
  local pattern="Text in cursive"

  ${DOCXBOX_BINARY} rmt "${PATH_DOCX_NEW}" "style" "cursive"

  source ./test/functional/_check_for_valgrind_errors.sh

  ${DOCXBOX_BINARY} txt "${PATH_DOCX_NEW}" | grep --count --invert-match "${pattern}"
}

@test "$BATS_TEST_NUMBER: Removing strings in the middle of a file within a sentence" {
  local pattern="A style"

  ${DOCXBOX_BINARY} rmt "${PATH_DOCX_NEW}" " paragraph" "special"

  source ./test/functional/_check_for_valgrind_errors.sh

  ${DOCXBOX_BINARY} txt "${PATH_DOCX_NEW}" | grep --count "${pattern}"
}

@test "$BATS_TEST_NUMBER: Removing strings w/ different styles" {
  ${DOCXBOX_BINARY} rmt "${PATH_DOCX_NEW}" "CAPS" "paragraph"

  source ./test/functional/_check_for_valgrind_errors.sh

  ${DOCXBOX_BINARY} txt "${PATH_DOCX_NEW}" | grep --count "without special style"
}

@test "$BATS_TEST_NUMBER: Removing strings at the end of a file" {
  local pattern="Bold text passages are great"

  local before_rmt
  before_rmt=$(${DOCXBOX_BINARY} txt "${PATH_DOCX_NEW}" | wc --words)

  ${DOCXBOX_BINARY} rmt "${PATH_DOCX_NEW}" "Bold" "great"

  source ./test/functional/_check_for_valgrind_errors.sh

  local after_rmt
  after_rmt=$(${DOCXBOX_BINARY} txt "${PATH_DOCX_NEW}" | wc --words)

  (( before_rmt > after_rmt ))
}

@test "$BATS_TEST_NUMBER: Trying to remove strings at the end of a file w/ a nonexistent string" {
  local pattern="Bold text passages are great"

  ${DOCXBOX_BINARY} rmt "${PATH_DOCX_NEW}" "Bold" "Foo"

  source ./test/functional/_check_for_valgrind_errors.sh

  ${DOCXBOX_BINARY} txt "${PATH_DOCX_NEW}" | grep --count "${pattern}"
}

@test "$BATS_TEST_NUMBER: Removing content between two given strings removes everything" {
  ${DOCXBOX_BINARY} lsi "${PATH_DOCX_STYLES}" | grep --count "image1.png"

  run "${DOCXBOX_BINARY}" rmt "${PATH_DOCX_STYLES}" "FROM" "Until"
  [ "$status" -eq 0 ]

  source ./test/functional/_check_for_valgrind_errors.sh

  ${DOCXBOX_BINARY} txt "${PATH_DOCX_STYLES}" | grep --count "I’m a dummy text file"
  ${DOCXBOX_BINARY} txt "${PATH_DOCX_STYLES}" | grep --count "next time also in BOLD"
  ${DOCXBOX_BINARY} txt "${PATH_DOCX_STYLES}" | grep --count --invert-match "I’m bold AND italic"

  ${DOCXBOX_BINARY} lsi "${PATH_DOCX_STYLES}" | grep --invert-match "image1.png"
}

@test "$BATS_TEST_NUMBER: \"${CMD} nonexistent.docx\" prints an error message" {
  run "${DOCXBOX_BINARY}" rmt nonexistent.docx
  [ "$status" -ne 0 ]

  ${DOCXBOX_BINARY} rmt nonexistent.docx Dolore incididunt 2>&1 | tee "${ERR_LOG}"
  source ./test/functional/_check_for_valgrind_errors.sh
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
    source ./test/functional/_check_for_valgrind_errors.sh
    cat "${ERR_LOG}" | grep --count "${pattern}"
  done
}
