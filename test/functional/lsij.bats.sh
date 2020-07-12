#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper
source ./test/functional/_set_docxbox_binary.sh

CMD="docxbox lsij"

PATH_DOCX="test/tmp/cp_table_unordered_list_images.docx"
ERR_LOG="test/tmp/err.log"

@test "${BATS_TEST_NUMBER}: Exit code of \"${CMD} filename.docx\" is zero" {
  run ${DOCXBOX_BINARY} lsij "${PATH_DOCX}"
  [ "$status" -eq 0 ]

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"${CMD} {missing argument}\" prints an error message" {
  run ${DOCXBOX_BINARY} lsij
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: DOCX filename" = "${lines[0]}" ]

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx\" displays contained images as JSON" {
  ${DOCXBOX_BINARY} lsij "${PATH_DOCX}" | grep --count "image2.jpeg"

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"docxbox lsi filename.docx --json\" displays contained images as JSON" {
  ${DOCXBOX_BINARY} lsi "${PATH_DOCX}" --json | grep --count "image2.jpeg"

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"docxbox lsi filename.docx -j\" displays contained images as JSON" {
  ${DOCXBOX_BINARY} lsi "${PATH_DOCX}" -j | grep --count "image2.jpeg"

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"docxbox ls filename.docx -ij\" displays contained images as JSON" {
  ${DOCXBOX_BINARY} lsi "${PATH_DOCX}" -ij | grep --count "image2.jpeg"

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"docxbox ls filename.docx --images --json\" displays contained images as JSON" {
  ${DOCXBOX_BINARY} ls "${PATH_DOCX}" --images --json | grep --count "image2.jpeg"

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"${CMD} nonexistent.docx\" prints an error message" {
  run ${DOCXBOX_BINARY} lsij nonexistent.docx
  [ "$status" -ne 0 ]

  source ./test/functional/_check_for_valgrind_errors.sh

  ${DOCXBOX_BINARY} lsij nonexistent.docx 2>&1 | tee "${ERR_LOG}"
  cat "${ERR_LOG}" | grep --count "docxBox Error - File not found:"
}

@test "${BATS_TEST_NUMBER}: \"${CMD} wrong_file_type\" prints an error message" {
  local wrong_file_types=(
  "test/tmp/cp_lorem_ipsum.pdf"
  "test/tmp/cp_mock_csv.csv"
  "test/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    ${DOCXBOX_BINARY} lsij "${i}" 2>&1 | tee "${ERR_LOG}"
    source ./test/functional/_check_for_valgrind_errors.sh
    cat "${ERR_LOG}" | grep --count "docxBox Error - File is no ZIP archive:"
  done
}

@test "${BATS_TEST_NUMBER}: First char of \"${CMD} filename.docx\"'s JSON is a \"[\"" {
  ${DOCXBOX_BINARY} lsij "${PATH_DOCX}" | grep "^[[]"
}

@test "${BATS_TEST_NUMBER}: Last char of \"${CMD} filename.docx\"'s JSON is a \"]\"" {
  ${DOCXBOX_BINARY} lsij "${PATH_DOCX}" | grep "[]]$"
}

@test "${BATS_TEST_NUMBER}: Amount opening and closing brackets \"[]\" must match" {
  local amount_opening amount_closing
  amount_opening=$(${DOCXBOX_BINARY} lsij "${PATH_DOCX}" | grep --count "\[")
  amount_closing=$(${DOCXBOX_BINARY} lsij "${PATH_DOCX}" | grep --count "\]")

  (( amount_opening = amount_closing ))
}

@test "${BATS_TEST_NUMBER}: Amount opening and closing brackets \"{}\" must match" {
  local amount_opening amount_closing
  amount_opening=$(${DOCXBOX_BINARY} lsij "${PATH_DOCX}" | grep --count "\{")
  amount_closing=$(${DOCXBOX_BINARY} lsij "${PATH_DOCX}" | grep --count "\}")

  (( amount_opening = amount_closing ))
}
