#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper
source ./test/functional/_set_docxbox_binary.sh

CMD="docxbox mm"

PATH_DOCX="test/tmp/cp_bio_assay.docx"
ERR_LOG="test/tmp/err.log"

@test "${BATS_TEST_NUMBER}: \"${CMD} {missing argument}\" prints an error message" {
  run "${DOCXBOX_BINARY}" mm
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: DOCX filename" = "${lines[0]}" ]

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx {missing argument}\" prints an error message" {
  local pattern="docxBox Error - Missing argument: Meta attribute to be set"

  run "${DOCXBOX_BINARY}" mm "${PATH_DOCX}"
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx title {argument}\" modifies meta attribute \"title\"" {
  run "${DOCXBOX_BINARY}" mm "${PATH_DOCX}" title "someTitle"
  [ "$status" -eq 0 ]

  source ./test/functional/_check_for_valgrind_errors.sh

  ${DOCXBOX_BINARY} lsm "${PATH_DOCX}" | grep --count "title: someTitle"
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx creator {argument}\" modifies meta attribute \"creator\"" {
  run "${DOCXBOX_BINARY}" mm "${PATH_DOCX}" creator "John Doe"
  [ "$status" -eq 0 ]

  source ./test/functional/_check_for_valgrind_errors.sh

  ${DOCXBOX_BINARY} lsm "${PATH_DOCX}" | grep --count "creator: John Doe"
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx lastModifiedBy {argument}\" modifies meta attribute \"lastModifiedBy\"" {
  local pattern="lastModifiedBy: John Doe"

  run "${DOCXBOX_BINARY}" mm "${PATH_DOCX}" lastModifiedBy "John Doe"
  [ "$status" -eq 0 ]

  source ./test/functional/_check_for_valgrind_errors.sh

  ${DOCXBOX_BINARY} lsm "${PATH_DOCX}" | grep --count "${pattern}"
}

@test "${BATS_TEST_NUMBER}: \"${CMD} lastPrinted {argument}\" modifies meta attribute \"lastPrinted\"" {
  local print_date="2020-02-20T10:31:00Z"
  local pattern="lastPrinted: 2020-02-20T10:31:00Z"

  run "${DOCXBOX_BINARY}" mm "${PATH_DOCX}" lastPrinted $print_date
  [ "$status" -eq 0 ]

  source ./test/functional/_check_for_valgrind_errors.sh

  ${DOCXBOX_BINARY} lsm "${PATH_DOCX}" | grep --count "${pattern}"
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx language {argument}\" modifies meta attribute \"language\"" {
  run "${DOCXBOX_BINARY}" mm "${PATH_DOCX}" language "de-CH"
  [ "$status" -eq 0 ]

  source ./test/functional/_check_for_valgrind_errors.sh

  ${DOCXBOX_BINARY} lsm "${PATH_DOCX}" | grep --count "language: de-CH"
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx created {argument}\" modifies meta attribute \"created\"" {
  local pattern="created: 2020-10-20T10:20:00Z"

  run "${DOCXBOX_BINARY}" mm "${PATH_DOCX}" created "2020-10-20T10:20:00Z"
  [ "$status" -eq 0 ]

  source ./test/functional/_check_for_valgrind_errors.sh

  ${DOCXBOX_BINARY} lsm "${PATH_DOCX}" | grep --count "${pattern}"
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx modified {argument}\" modifies meta attribute \"modified\"" {
  local pattern="modified: 2020-10-20T10:20:00Z"

  run "${DOCXBOX_BINARY}" mm "${PATH_DOCX}" modified "2020-10-20T10:20:00Z"
  [ "$status" -eq 0 ]

  source ./test/functional/_check_for_valgrind_errors.sh

  ${DOCXBOX_BINARY} lsm "${PATH_DOCX}" | grep --count "${pattern}"
}

@test "${BATS_TEST_NUMBER}: Modifying the meta attribute \"created\" does not change the meta attribute \"modified\"" {
  local created
  local created=$(${DOCXBOX_BINARY} lsm "${PATH_DOCX}" | grep "created")

  run "${DOCXBOX_BINARY}" mm "${PATH_DOCX}" modified "2020-10-20T10:20:00Z"
  [ "$status" -eq 0 ]

  source ./test/functional/_check_for_valgrind_errors.sh

  ${DOCXBOX_BINARY} lsm "${PATH_DOCX}" | grep "${created}"
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx revision {argument}\" modifies meta attribute \"revision\"" {
  run "${DOCXBOX_BINARY}" mm "${PATH_DOCX}" revision "25"
  [ "$status" -eq 0 ]

  source ./test/functional/_check_for_valgrind_errors.sh

  ${DOCXBOX_BINARY} lsm "${PATH_DOCX}" | grep "revision: 25"
}


@test "${BATS_TEST_NUMBER}: \"${CMD} nonexistent.docx\" prints an error message" {
  run "${DOCXBOX_BINARY}" mm nonexistent.docx
  [ "$status" -ne 0 ]
  source ./test/functional/_check_for_valgrind_errors.sh

  ${DOCXBOX_BINARY} mm nonexistent.docx revision "1" 2>&1 | tee "${ERR_LOG}"
  cat "${ERR_LOG}" | grep --count "docxBox Error - File not found:"
}

@test "${BATS_TEST_NUMBER}: \"${CMD} {meta_attribute} {argument} wrong_file_type\" prints an error message" {
  local pattern="docxBox Error - File is no ZIP archive:"
  local wrong_file_types=(
  "test/tmp/cp_lorem_ipsum.pdf"
  "test/tmp/cp_mock_csv.csv"
  "test/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    ${DOCXBOX_BINARY} lorem "${i}" revision "12" 2>&1 | tee "${ERR_LOG}"
    source ./test/functional/_check_for_valgrind_errors.sh
    cat "${ERR_LOG}" | grep --count "${pattern}"
  done
}
