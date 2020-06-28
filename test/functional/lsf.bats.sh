#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

VALGRIND="valgrind -v --leak-check=full\
 --log-file=test/assets/documents/other/mem-leak.log"

if $IS_VALGRIND_TEST;
then
  DOCXBOX_BINARY="${VALGRIND} $BATS_TEST_DIRNAME/../tmp/docxbox"
else
  DOCXBOX_BINARY="$BATS_TEST_DIRNAME/../tmp/docxbox"
fi

PATH_DOCX="test/tmp/cp_mergefields.docx"
ERR_LOG="test/tmp/err.log"

BASE_COMMAND="docxbox lsf filename.docx"
LONGHAND_COMMAND="docxbox ls filename.docx"

@test "Exit code of \"${BASE_COMMAND}\" is zero" {
  run ${DOCXBOX_BINARY} lsf "${PATH_DOCX}"
  [ "$status" -eq 0 ]
}

@test "Output of \"docxbox lsf {missing argument}\" is an error message" {
  pattern="docxBox Error - Missing argument: Filename of DOCX to be extracted"

  run ${DOCXBOX_BINARY} lsf
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]
}

@test "Output of \"${BASE_COMMAND}\" contains ground informations" {
  run ${DOCXBOX_BINARY} lsf "${PATH_DOCX}"
  [ "$status" -eq 0 ]
  [ "word/fontTable.xml lists 10 fonts:" = "${lines[0]}" ]
}

@test "Output of \"${LONGHAND_COMMAND} --fonts\" contains ground informations" {
  run ${DOCXBOX_BINARY} ls "${PATH_DOCX}" --fonts
  [ "$status" -eq 0 ]
  [ "word/fontTable.xml lists 10 fonts:" = "${lines[0]}" ]
}

@test "Output of \"${LONGHAND_COMMAND} -f\" contains ground informations" {
  run ${DOCXBOX_BINARY} ls "${PATH_DOCX}" -f
  [ "$status" -eq 0 ]
  [ "word/fontTable.xml lists 10 fonts:" = "${lines[0]}" ]
}

@test "Output of \"${BASE_COMMAND}\" contains files' and directories' attributes" {
  attributes=(
  "Font"
  "AltName"
  "CharSet"
  "Family"
  "Pitch")

  for i in "${attributes[@]}"
  do
    ${DOCXBOX_BINARY} lsf "${PATH_DOCX}" | grep --count "${i}"
  done
}

@test "Output of \"${BASE_COMMAND}\" contains fontfile-filename" {
  ${DOCXBOX_BINARY} lsf "${PATH_DOCX}" | grep --count "fontTable.xml"
}

@test "Output of \"${BASE_COMMAND}\" contains amount fonts" {
  ${DOCXBOX_BINARY} lsf "${PATH_DOCX}" | grep --count "10 fonts"
}

@test "Output of \"${BASE_COMMAND}\" contains font names" {
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
    ${DOCXBOX_BINARY} lsf "${PATH_DOCX}" | grep --count "${i}"
  done
}

@test "Output of \"${BASE_COMMAND}\" can contain alternative font names" {
  ${DOCXBOX_BINARY} lsf "${PATH_DOCX}" | grep --count "宋体"
}

@test "Output of \"${BASE_COMMAND}\" contains font-charSets" {
  ${DOCXBOX_BINARY} lsf "${PATH_DOCX}" | grep --count "00"
}

@test "Output of \"${BASE_COMMAND}\" contains font-family" {
  font_family=(
  "roman"
  "swiss"
  "auto")

  for i in "${font_family[@]}"
  do
    ${DOCXBOX_BINARY} lsf "${PATH_DOCX}" | grep --count "${i}"
  done
}

@test "Output of \"${BASE_COMMAND}\" contains font-pitch" {
  ${DOCXBOX_BINARY} lsf "${PATH_DOCX}" | grep --count "variable"
}

@test "Output of \"docxbox lsf nonexistent.docx\" is an error message" {
  run ${DOCXBOX_BINARY} lsf nonexistent.docx
  [ "$status" -ne 0 ]

  ${DOCXBOX_BINARY} lsf nonexistent.docx 2>&1 | tee "${ERR_LOG}"
  cat "${ERR_LOG}" | grep --count "docxBox Error - File not found:"
}

@test "Output of \"docxbox lsf wrong_file_type\" is an error message" {
  wrong_file_types=(
  "test/tmp/cp_lorem_ipsum.pdf"
  "test/tmp/cp_mock_csv.csv"
  "test/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    ${DOCXBOX_BINARY} lsf "${i}" 2>&1 | tee "${ERR_LOG}"
    cat "${ERR_LOG}" | grep --count "docxBox Error - File is no ZIP archive:"
  done
}
