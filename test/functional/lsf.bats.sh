#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

CMD="docxbox lsf"

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

@test "${BATS_TEST_NUMBER}: Exit code of \"${CMD}\" is zero" {
  run ${DOCXBOX_BINARY} lsf "${PATH_DOCX}"
  [ "$status" -eq 0 ]

  check_for_valgrind_error
}

@test "${BATS_TEST_NUMBER}: \"${CMD} {missing argument}\" prints an error message" {
  local pattern="docxBox Error - Missing argument: Filename of DOCX to be extracted"

  run ${DOCXBOX_BINARY} lsf
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]

  check_for_valgrind_error
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx\" displays ground information" {
  run ${DOCXBOX_BINARY} lsf "${PATH_DOCX}"
  [ "$status" -eq 0 ]
  [ "word/fontTable.xml lists 10 fonts:" = "${lines[0]}" ]

  check_for_valgrind_error
}

@test "${BATS_TEST_NUMBER}: \"docxbox ls filename.docx --fonts\" displays ground information" {
  run ${DOCXBOX_BINARY} ls "${PATH_DOCX}" --fonts
  [ "$status" -eq 0 ]
  [ "word/fontTable.xml lists 10 fonts:" = "${lines[0]}" ]

  check_for_valgrind_error
}

@test "${BATS_TEST_NUMBER}: \"docxbox ls filename.docx -f\" displays ground information" {
  run ${DOCXBOX_BINARY} ls "${PATH_DOCX}" -f
  [ "$status" -eq 0 ]
  [ "word/fontTable.xml lists 10 fonts:" = "${lines[0]}" ]

  check_for_valgrind_error
}

@test "${BATS_TEST_NUMBER}: \"${CMD}\" displays files' and directories' attributes" {
  local attributes=(
  "Font"
  "AltName"
  "CharSet"
  "Family"
  "Pitch")

  for i in "${attributes[@]}"
  do
    ${DOCXBOX_BINARY} lsf "${PATH_DOCX}" | grep --count "${i}"
    check_for_valgrind_error
  done
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx\" displays fontfile-filename" {
  ${DOCXBOX_BINARY} lsf "${PATH_DOCX}" | grep --count "fontTable.xml"

  check_for_valgrind_error
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx\" displays amount fonts" {
  ${DOCXBOX_BINARY} lsf "${PATH_DOCX}" | grep --count "10 fonts"

  check_for_valgrind_error
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
    ${DOCXBOX_BINARY} lsf "${PATH_DOCX}" | grep --count "${i}"
    check_for_valgrind_error
  done
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx\" displays alternative font names if available" {
  ${DOCXBOX_BINARY} lsf "${PATH_DOCX}" | grep --count "宋体"

  check_for_valgrind_error
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx\" displays font-charSets" {
  ${DOCXBOX_BINARY} lsf "${PATH_DOCX}" | grep --count "00"

  check_for_valgrind_error
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx\" displays font-family" {
  local font_family=(
  "roman"
  "swiss"
  "auto")

  for i in "${font_family[@]}"
  do
    ${DOCXBOX_BINARY} lsf "${PATH_DOCX}" | grep --count "${i}"
    check_for_valgrind_error
  done
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx\" displays font-pitch" {
  ${DOCXBOX_BINARY} lsf "${PATH_DOCX}" | grep --count "variable"

  check_for_valgrind_error
}

@test "${BATS_TEST_NUMBER}: \"${CMD} nonexistent.docx\" prints an error message" {
  run ${DOCXBOX_BINARY} lsf nonexistent.docx
  [ "$status" -ne 0 ]

  check_for_valgrind_error

  ${DOCXBOX_BINARY} lsf nonexistent.docx 2>&1 | tee "${ERR_LOG}"
  cat "${ERR_LOG}" | grep --count "docxBox Error - File not found:"
}

@test "${BATS_TEST_NUMBER}: \"${CMD} wrong_file_type\" prints an error message" {
  local wrong_file_types=(
  "test/tmp/cp_lorem_ipsum.pdf"
  "test/tmp/cp_mock_csv.csv"
  "test/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    ${DOCXBOX_BINARY} lsf "${i}" 2>&1 | tee "${ERR_LOG}"
    cat "${ERR_LOG}" | grep --count "docxBox Error - File is no ZIP archive:"
    check_for_valgrind_error
  done
}
