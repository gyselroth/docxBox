#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper
source ./test/functional/_set_docxbox_binary.sh

CMD="docxbox lsj"

PATH_DOCX="test/tmp/cp_table_unordered_list_images.docx"
ERR_LOG="test/tmp/err.log"

ATTRIBUTES=(
  "length"
  "date"
  "time"
  "file")

@test "$BATS_TEST_NUMBER: Exit code of \"${CMD} filename.docx\" is zero" {
  run ${DOCXBOX_BINARY} lsj "${PATH_DOCX}"
  [ "$status" -eq 0 ]

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"${CMD} {missing argument}\" prints an error message" {
  run ${DOCXBOX_BINARY} lsj
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: DOCX filename" = "${lines[0]}" ]

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx\" displays files' and directories' attributes" {
  for i in "${ATTRIBUTES[@]}"
  do
    ${DOCXBOX_BINARY} lsj "${PATH_DOCX}" | grep --count "${i}"
    source ./test/functional/_check_for_valgrind_errors.sh
  done
}

@test "${BATS_TEST_NUMBER}: \"docxbox ls filename.docx --json\" displays files' and directories' attributes" {
  for i in "${ATTRIBUTES[@]}"
  do
    ${DOCXBOX_BINARY} ls "${PATH_DOCX}" --json | grep --count "${i}"
    source ./test/functional/_check_for_valgrind_errors.sh
  done
}

@test "${BATS_TEST_NUMBER}: \"docxbox ls filename.docx -j\" displays files' and directories' attributes" {
  for i in "${ATTRIBUTES[@]}"
  do
    ${DOCXBOX_BINARY} ls "${PATH_DOCX}" -j | grep --count "${i}"
    source ./test/functional/_check_for_valgrind_errors.sh
  done
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx\" displays contained files" {
local search_values=(
  "[Content_Types].xml"
  "_rels/.rels"
  "word/_rels/document.xml.rels"
  "word/document.xml"
  "word/theme/theme1.xml"
  "word/media/image1.png"
  "word/media/image2.jpeg"
  "word/settings.xml"
  "word/webSettings.xml"
  "word/styles.xml"
  "word/numbering.xml"
  "docProps/core.xml"
  "word/fontTable.xml"
  "docProps/app.xml")

  for i in "${search_values[@]}"
  do
    ${DOCXBOX_BINARY} lsj "${PATH_DOCX}" | grep --count "${i}"
    source ./test/functional/_check_for_valgrind_errors.sh
  done
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx\" displays files' date and time" {
  ${DOCXBOX_BINARY} lsj "${PATH_DOCX}" | grep --count "7/./2020"
  ${DOCXBOX_BINARY} lsj "${PATH_DOCX}" | grep --count "7/3/2020"

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"${CMD} nonexistent.docx\" prints an error message" {
  run ${DOCXBOX_BINARY} lsj nonexistent.docx
  [ "$status" -ne 0 ]

  source ./test/functional/_check_for_valgrind_errors.sh

  ${DOCXBOX_BINARY} lsj nonexistent.docx 2>&1 | tee "${ERR_LOG}"
  cat "${ERR_LOG}" | grep --count "docxBox Error - File not found:"
}

@test "${BATS_TEST_NUMBER}: \"${CMD} wrong_file_type\" prints an error message" {
  local wrong_file_types=(
  "test/tmp/cp_lorem_ipsum.pdf"
  "test/tmp/cp_mock_csv.csv"
  "test/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    ${DOCXBOX_BINARY} lsj "${i}" 2>&1 | tee "${ERR_LOG}"
    source ./test/functional/_check_for_valgrind_errors.sh
    cat "${ERR_LOG}" | grep --count "docxBox Error - File is no ZIP archive:"
  done
}

@test "${BATS_TEST_NUMBER}: First char of \"${CMD} filename.docx\"'s JSON is a \"[\"" {
  ${DOCXBOX_BINARY} lsj "${PATH_DOCX}" | grep "^[[]"
}

@test "${BATS_TEST_NUMBER}: Last char of \"${CMD} filename.docx\"'s JSON is a \"]\"" {
  ${DOCXBOX_BINARY} lsj "${PATH_DOCX}" | grep "[]]$"
}

@test "${BATS_TEST_NUMBER}: Amount opening and closing brackets \"[]\" must match" {
  local amount_opening amount_closing
  amount_opening=$(${DOCXBOX_BINARY} lsj "${PATH_DOCX}" | grep --count "\[")
  amount_closing=$(${DOCXBOX_BINARY} lsj "${PATH_DOCX}" | grep --count "\]")

  (( amount_opening = amount_closing ))
}

@test "${BATS_TEST_NUMBER}: Amount opening and closing brackets \"{}\" must match" {
  local amount_opening amount_closing
  amount_opening=$(${DOCXBOX_BINARY} lsj "${PATH_DOCX}" | grep --count "\{")
  amount_closing=$(${DOCXBOX_BINARY} lsj "${PATH_DOCX}" | grep --count "\}")

  (( amount_opening = amount_closing ))
}
