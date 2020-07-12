#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper
source ./test/functional/_set_docxbox_binary.sh

CMD="docxbox lsfj"

PATH_DOCX="test/tmp/cp_mergefields.docx"
ERR_LOG="test/tmp/err.log"

ATTRIBUTES=(
  "font"
  "altName"
  "charset"
  "family"
  "pitch")

@test "${BATS_TEST_NUMBER}: Exit code of \"${CMD} filename.docx\" is zero" {
  run ${DOCXBOX_BINARY} lsfj "${PATH_DOCX}"
  [ "$status" -eq 0 ]

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"${CMD} {missing argument}\" prints an error message" {
  run ${DOCXBOX_BINARY} lsfj
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: Filename of DOCX to be extracted" = "${lines[0]}" ]

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"${CMD}\" displays files' and directories' attributes" {
  for i in "${ATTRIBUTES[@]}"
  do
    ${DOCXBOX_BINARY} lsfj "${PATH_DOCX}" | grep --count "${i}"
    source ./test/functional/_check_for_valgrind_errors.sh
  done
}

@test "${BATS_TEST_NUMBER}: \"docxbox lsf --json\" displays files' and directories' attributes" {
  for i in "${ATTRIBUTES[@]}"
  do
    ${DOCXBOX_BINARY} lsf "${PATH_DOCX}" --json | grep --count "${i}"
    source ./test/functional/_check_for_valgrind_errors.sh
  done
}

@test "${BATS_TEST_NUMBER}: \"docxbox lsf -j\" displays files' and directories' attributes" {
  for i in "${ATTRIBUTES[@]}"
  do
    ${DOCXBOX_BINARY} lsf "${PATH_DOCX}" -j | grep --count "${i}"
    source ./test/functional/_check_for_valgrind_errors.sh
  done
}

@test "${BATS_TEST_NUMBER}: \"docxbox ls filename.docx --fonts --json\" displays files' and directories' attributes" {
  for i in "${ATTRIBUTES[@]}"
  do
    ${DOCXBOX_BINARY} ls "${PATH_DOCX}" --fonts --json | grep --count "${i}"
    source ./test/functional/_check_for_valgrind_errors.sh
  done
}

@test "${BATS_TEST_NUMBER}: \"docxbox ls filename.docx -fj\" displays files' and directories' attributes" {
  for i in "${ATTRIBUTES[@]}"
  do
    ${DOCXBOX_BINARY} ls "${PATH_DOCX}" -fj | grep --count "${i}"
    source ./test/functional/_check_for_valgrind_errors.sh
  done
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx\" displays fontfile-filename" {
  ${DOCXBOX_BINARY} lsfj "${PATH_DOCX}" | grep --count "fontTable.xml"

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx\" displays font names" {
  local font_names=(
    "Calibri
    Times New Roman
    Arial
    MS Mincho
    Arial Black
    Verdana
    Times
    Calibri Light")

  for i in "${font_names[@]}"
  do
    ${DOCXBOX_BINARY} lsfj "${PATH_DOCX}" | grep --count "${i}"
    source ./test/functional/_check_for_valgrind_errors.sh
  done
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx\" displays alternative font names if available" {
  ${DOCXBOX_BINARY} lsfj "${PATH_DOCX}" | grep --count "宋体"

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx\" displays font-charSets" {
  ${DOCXBOX_BINARY} lsfj "${PATH_DOCX}" | grep --count "00"

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx\" displays font-family" {
  local font_family=(
  "roman"
  "swiss"
  "auto")

  for i in "${font_family[@]}"
  do
    ${DOCXBOX_BINARY} lsfj "${PATH_DOCX}" | grep --count "${i}"
    source ./test/functional/_check_for_valgrind_errors.sh
  done
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx\" displays font-pitch" {
  ${DOCXBOX_BINARY} lsfj "${PATH_DOCX}" | grep --count "variable"

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"docxbox lsfj nonexistent.docx\" prints an error message" {
  run ${DOCXBOX_BINARY} lsfj nonexistent.docx
  [ "$status" -ne 0 ]

  source ./test/functional/_check_for_valgrind_errors.sh

  ${DOCXBOX_BINARY} lsfj nonexistent.docx 2>&1 | tee "${ERR_LOG}"
  cat "${ERR_LOG}" | grep --count "docxBox Error - File not found:"
}

@test "${BATS_TEST_NUMBER}: \"docxbox lsfj wrong_file_type\" prints an error message" {
  local wrong_file_types=(
  "test/tmp/cp_lorem_ipsum.pdf"
  "test/tmp/cp_mock_csv.csv"
  "test/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    ${DOCXBOX_BINARY} lsfj "${i}" 2>&1 | tee "${ERR_LOG}"
    source ./test/functional/_check_for_valgrind_errors.sh
    cat "${ERR_LOG}" | grep --count "docxBox Error - File is no ZIP archive:"
  done
}

@test "${BATS_TEST_NUMBER}: First char of \"${CMD} filename.docx\"'s JSON is a \"[\"" {
  ${DOCXBOX_BINARY} lsfj "${PATH_DOCX}" | grep "^[[]"
}

@test "${BATS_TEST_NUMBER}: Last char of \"${CMD} filename.docx\"'s JSON is a \"]\"" {
  ${DOCXBOX_BINARY} lsfj "${PATH_DOCX}" | grep "[]]$"
}

@test "${BATS_TEST_NUMBER}: Amount opening and closing brackets \"[]\" must match" {
  local amount_opening amount_closing
  amount_opening=$(${DOCXBOX_BINARY} lsfj "${PATH_DOCX}" | grep --count "\[")
  amount_closing=$(${DOCXBOX_BINARY} lsfj "${PATH_DOCX}" | grep --count "\]")

  (( amount_opening = amount_closing ))
}

@test "${BATS_TEST_NUMBER}: Amount opening and closing brackets \"{}\" must match" {
  local amount_opening amount_closing
  amount_opening=$(${DOCXBOX_BINARY} lsfj "${PATH_DOCX}" | grep --count "\{")
  amount_closing=$(${DOCXBOX_BINARY} lsfj "${PATH_DOCX}" | grep --count "\}")

  (( amount_opening = amount_closing ))
}
