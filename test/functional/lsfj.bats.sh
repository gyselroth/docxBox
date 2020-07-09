#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

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

PATH_DOCX="test/tmp/cp_mergefields.docx"
ERR_LOG="test/tmp/err.log"

BASE_COMMAND="docxbox lsfj filename.docx"
LONGHAND_COMMAND="docxbox lsf filename.docx"

LONG_DESCRIPTION="contains files' and directories' attributes"

ATTRIBUTES=(
  "font"
  "altName"
  "charset"
  "family"
  "pitch")

@test "Case 1: Exit code of \"docxbox ls filename.docx\" is zero" {
  run ${DOCXBOX_BINARY} lsfj "${PATH_DOCX}"
  [ "$status" -eq 0 ]

  check_for_valgrind_error
}

@test "Case 2: Output of \"docxbox lsfj {missing argument}\" is an error message" {
  run ${DOCXBOX_BINARY} lsfj
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: Filename of DOCX to be extracted" = "${lines[0]}" ]

  check_for_valgrind_error
}

@test "Case 3: Output of \"${BASE_COMMAND}\" ${LONG_DESCRIPTION}" {
  for i in "${ATTRIBUTES[@]}"
  do
    ${DOCXBOX_BINARY} lsfj "${PATH_DOCX}" | grep --count "${i}"
    check_for_valgrind_error
  done
}

@test "Case 4: Output of \"${LONGHAND_COMMAND} --json\" ${LONG_DESCRIPTION}" {
  for i in "${ATTRIBUTES[@]}"
  do
    ${DOCXBOX_BINARY} lsf "${PATH_DOCX}" --json | grep --count "${i}"
    check_for_valgrind_error
  done
}

@test "Case 5: Output of \"${LONGHAND_COMMAND} -j\" ${LONG_DESCRIPTION}" {
  for i in "${ATTRIBUTES[@]}"
  do
    ${DOCXBOX_BINARY} lsf "${PATH_DOCX}" -j | grep --count "${i}"
    check_for_valgrind_error
  done
}

LONGHAND="--fonts --json"
@test "Case 6: Output of \"docxbox ls filename.docx ${LONGHAND}\" ${LONG_DESCRIPTION}" {
  for i in "${ATTRIBUTES[@]}"
  do
    ${DOCXBOX_BINARY} ls "${PATH_DOCX}" ${LONGHAND} | grep --count "${i}"
    check_for_valgrind_error
  done
}

@test "Case 7: Output of \"docxbox ls filename.docx -fj\" ${LONG_DESCRIPTION}" {
  for i in "${ATTRIBUTES[@]}"
  do
    ${DOCXBOX_BINARY} ls "${PATH_DOCX}" -fj | grep --count "${i}"
    check_for_valgrind_error
  done
}

@test "Case 8: Output of \"${BASE_COMMAND}\" contains fontfile-filename" {
  ${DOCXBOX_BINARY} lsfj "${PATH_DOCX}" | grep --count "fontTable.xml"

  check_for_valgrind_error
}

@test "Case 9: Output of \"${BASE_COMMAND}\" contains font names" {
  font_names=(
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
    check_for_valgrind_error
  done
}

@test "Case 10: Output of \"${BASE_COMMAND}\" can contain alternative font names" {
  ${DOCXBOX_BINARY} lsfj "${PATH_DOCX}" | grep --count "宋体"

  check_for_valgrind_error
}

@test "Case 11: Output of \"${BASE_COMMAND}\" contains font-charSets" {
  ${DOCXBOX_BINARY} lsfj "${PATH_DOCX}" | grep --count "00"

  check_for_valgrind_error
}

@test "Case 12: Output of \"${BASE_COMMAND}\" contains font-family" {
  font_family=(
  "roman"
  "swiss"
  "auto")

  for i in "${font_family[@]}"
  do
    ${DOCXBOX_BINARY} lsfj "${PATH_DOCX}" | grep --count "${i}"
    check_for_valgrind_error
  done
}

@test "Case 13: Output of \"${BASE_COMMAND}\" contains font-pitch" {
  ${DOCXBOX_BINARY} lsfj "${PATH_DOCX}" | grep --count "variable"

  check_for_valgrind_error
}

@test "Case 14: Output of \"docxbox lsfj nonexistent.docx\" is an error message" {
  run ${DOCXBOX_BINARY} lsfj nonexistent.docx
  [ "$status" -ne 0 ]

  check_for_valgrind_error

  ${DOCXBOX_BINARY} lsfj nonexistent.docx 2>&1 | tee "${ERR_LOG}"
  cat "${ERR_LOG}" | grep --count "docxBox Error - File not found:"
}

@test "Case 15: Output of \"docxbox lsfj wrong_file_type\" is an error message" {
  wrong_file_types=(
  "test/tmp/cp_lorem_ipsum.pdf"
  "test/tmp/cp_mock_csv.csv"
  "test/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    ${DOCXBOX_BINARY} lsfj "${i}" 2>&1 | tee "${ERR_LOG}"
    check_for_valgrind_error
    cat "${ERR_LOG}" | grep --count "docxBox Error - File is no ZIP archive:"
  done
}

title_first_char="Case 16: First char of the output of \"./docxbox lsfj \
filename.docx\"'s JSON is a \"[\""
@test "${title_first_char}" {
  ${DOCXBOX_BINARY} lsfj "${PATH_DOCX}" | grep "^[[]"
}

title_last_char="Case 17: Last char of the output of \"./docxbox lsfj \
filename.docx\"'s JSON is a \"]\""
@test "${title_last_char}" {
  ${DOCXBOX_BINARY} lsfj "${PATH_DOCX}" | grep "[]]$"
}

@test "Case 18: Amount opening and closing brackets \"[]\" must match" {
  amount_opening=$(${DOCXBOX_BINARY} lsfj "${PATH_DOCX}" | grep --count "\[")
  amount_closing=$(${DOCXBOX_BINARY} lsfj "${PATH_DOCX}" | grep --count "\]")

  (( amount_opening = amount_closing ))
}

@test "Case 19: Amount opening and closing brackets \"{}\" must match" {
  amount_opening=$(${DOCXBOX_BINARY} lsfj "${PATH_DOCX}" | grep --count "\{")
  amount_closing=$(${DOCXBOX_BINARY} lsfj "${PATH_DOCX}" | grep --count "\}")

  (( amount_opening = amount_closing ))
}
