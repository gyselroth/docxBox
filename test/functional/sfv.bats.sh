#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper
source ./test/functional/_set_docxbox_binary.sh

CMD="docxbox sfv"

PATH_DOCX="test/tmp/cp_mergefields.docx"
ERR_LOG="test/tmp/err.log"

MERGEFIELD="MERGEFIELD  Mergefield_One"
MERGEFIELD_HEADER="MERGEFIELD  Mergefield_Header"
MERGEFIELD_FOOTER="MERGEFIELD  Mergefield_Footer"

@test "${BATS_TEST_NUMBER}: \"${CMD} {missing argument}\" prints an error message" {
  local pattern="docxBox Error - Missing argument: Filename of DOCX to be extracted"

  run ${DOCXBOX_BINARY} sfv
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx {missing argument}\" prints an error message" {
  run ${DOCXBOX_BINARY} sfv "${PATH_DOCX}"
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: Field identifier" = "${lines[0]}" ]

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx fieldIdentifier {missing argument}\" prints an error message" {
  run ${DOCXBOX_BINARY} sfv "${PATH_DOCX}" "${MERGEFIELD}"
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: Value to be set" = "${lines[0]}" ]

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx fieldIdentifier fieldValue\" changes value of given field" {
  run ${DOCXBOX_BINARY} sfv "${PATH_DOCX}" "${MERGEFIELD}" foobar
  [ "$status" -eq 0 ]

  source ./test/functional/_check_for_valgrind_errors.sh

  ${DOCXBOX_BINARY} txt "${PATH_DOCX}" | grep --count "foobar"
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx fieldIdentifier fieldValue\" changes value of MERGEFIELD within header" {
  run ${DOCXBOX_BINARY} sfv "${PATH_DOCX}" "${MERGEFIELD_HEADER}" foobar
  [ "$status" -eq 0 ]

  source ./test/functional/_check_for_valgrind_errors.sh

  ${DOCXBOX_BINARY} txt "${PATH_DOCX}" | grep --count "foobar"
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx fieldIdentifier fieldValue\" changes value of MERGEFIELD within footer" {
  run ${DOCXBOX_BINARY} sfv "${PATH_DOCX}" "${MERGEFIELD_FOOTER}" foobar
  [ "$status" -eq 0 ]

  source ./test/functional/_check_for_valgrind_errors.sh

  ${DOCXBOX_BINARY} txt "${PATH_DOCX}" | grep --count "foobar"
}

@test "${BATS_TEST_NUMBER}: \"${CMD} nonexistent.docx\" prints an error message" {
  run ${DOCXBOX_BINARY} sfv nonexistent.docx
  [ "$status" -ne 0 ]

  source ./test/functional/_check_for_valgrind_errors.sh

  ${DOCXBOX_BINARY} sfv nonexistent.docx "${MERGEFIELD}" foobar 2>&1 | tee "${ERR_LOG}"
  cat "${ERR_LOG}" | grep --count "docxBox Error - File not found:"
}

@test "${BATS_TEST_NUMBER}: \"${CMD} wrong_file_type fieldIdentifier fieldValue\" prints an error message" {
  pattern="docxBox Error - File is no ZIP archive:"
  wrong_file_types=(
  "test/tmp/cp_lorem_ipsum.pdf"
  "test/tmp/cp_mock_csv.csv"
  "test/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    ${DOCXBOX_BINARY} lorem "${i}" "${MERGEFIELD}" foobar 2>&1 | tee "${ERR_LOG}"
    source ./test/functional/_check_for_valgrind_errors.sh
    cat "${ERR_LOG}" | grep --count "${pattern}"
  done
}
