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

path_docx="test/tmp/cp_mergefields.docx"

base_command="docxbox lsf filename.docx"
longhand_command="docxbox ls filename.docx"

@test "Exit code of \"${base_command}\" is zero" {
  run ${DOCXBOX_BINARY} lsf "${path_docx}"
  [ "$status" -eq 0 ]
}

@test "Output of \"docxbox lsf {missing argument}\" is an error message" {
  pattern="docxBox Error - Missing argument: Filename of DOCX to be extracted"

  run ${DOCXBOX_BINARY} lsf
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]
}

@test "Output of \"${base_command}\" contains ground informations" {
  run ${DOCXBOX_BINARY} lsf "${path_docx}"
  [ "$status" -eq 0 ]
  [ "word/fontTable.xml lists 10 fonts:" = "${lines[0]}" ]
}

@test "Output of \"${longhand_command} --fonts\" contains ground informations" {
  run ${DOCXBOX_BINARY} ls "${path_docx}" --fonts
  [ "$status" -eq 0 ]
  [ "word/fontTable.xml lists 10 fonts:" = "${lines[0]}" ]
}

@test "Output of \"${longhand_command} -f\" contains ground informations" {
  run ${DOCXBOX_BINARY} ls "${path_docx}" -f
  [ "$status" -eq 0 ]
  [ "word/fontTable.xml lists 10 fonts:" = "${lines[0]}" ]
}

@test "Output of \"${base_command}\" contains files' and directories' attributes" {
  attributes=(
  "Font"
  "AltName"
  "CharSet"
  "Family"
  "Pitch")

  for i in "${attributes[@]}"
  do
    ${DOCXBOX_BINARY} lsf "${path_docx}" | grep --count "${i}"
  done
}

@test "Output of \"${base_command}\" contains fontfile-filename" {
  ${DOCXBOX_BINARY} lsf "${path_docx}" | grep --count "fontTable.xml"
}

@test "Output of \"${base_command}\" contains amount fonts" {
  ${DOCXBOX_BINARY} lsf "${path_docx}" | grep --count "10 fonts"
}

@test "Output of \"${base_command}\" contains font names" {
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
    ${DOCXBOX_BINARY} lsf "${path_docx}" | grep --count "${i}"
  done
}

@test "Output of \"${base_command}\" can contain alternative font names" {
  ${DOCXBOX_BINARY} lsf "${path_docx}" | grep --count "宋体"
}

@test "Output of \"${base_command}\" contains font-charSets" {
  ${DOCXBOX_BINARY} lsf "${path_docx}" | grep --count "00"
}

@test "Output of \"${base_command}\" contains font-family" {
  font_family=(
  "roman"
  "swiss"
  "auto")

  for i in "${font_family[@]}"
  do
    ${DOCXBOX_BINARY} lsf "${path_docx}" | grep --count "${i}"
  done
}

@test "Output of \"${base_command}\" contains font-pitch" {
  ${DOCXBOX_BINARY} lsf "${path_docx}" | grep --count "variable"
}

@test "Output of \"docxbox lsf nonexistent.docx\" is an error message" {
  err_log="test/tmp/err.log"

  run ${DOCXBOX_BINARY} lsf nonexistent.docx
  [ "$status" -ne 0 ]

  ${DOCXBOX_BINARY} lsf nonexistent.docx 2>&1 | tee "${err_log}"
  cat "${err_log}" | grep --count "docxBox Error - File not found:"
}

@test "Output of \"docxbox lsf wrong_file_type\" is an error message" {
  err_log="test/tmp/err.log"
  wrong_file_types=(
  "test/tmp/cp_lorem_ipsum.pdf"
  "test/tmp/cp_mock_csv.csv"
  "test/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    ${DOCXBOX_BINARY} lsf "${i}" 2>&1 | tee "${err_log}"
    cat "${err_log}" | grep --count "docxBox Error - File is no ZIP archive:"
  done
}
