#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

docxbox=""$BATS_TEST_DIRNAME"/docxbox"
path_docx="test/functional/tmp/cp_mergefields.docx"

base_command="docxbox lsf filename.docx"
longhand_command="docxbox ls filename.docx"

@test "Exit code of \"${base_command}\" is zero" {
  run "${docxbox}" lsf "${path_docx}"
  [ "$status" -eq 0 ]
}

@test "Output of \"docxbox lsf {missing argument}\" is an error message" {
  pattern="docxBox Error - Missing argument: Filename of DOCX to be extracted"

  run "${docxbox}" lsf
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]
}

@test "Output of \"${base_command}\" contains ground informations" {
  run "${docxbox}" lsf "${path_docx}"
  [ "$status" -eq 0 ]
  [ "word/fontTable.xml lists 8 fonts:" = "${lines[0]}" ]
}

@test "Output of \"${longhand_command} --fonts\" contains ground informations" {
  run "${docxbox}" ls "${path_docx}" --fonts
  [ "$status" -eq 0 ]
  [ "word/fontTable.xml lists 8 fonts:" = "${lines[0]}" ]
}

@test "Output of \"${longhand_command} -f\" contains ground informations" {
  run "${docxbox}" ls "${path_docx}" -f
  [ "$status" -eq 0 ]
  [ "word/fontTable.xml lists 8 fonts:" = "${lines[0]}" ]
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
    "${docxbox}" lsf "${path_docx}" | grep --count "${i}"
  done
}

@test "Output of \"${base_command}\" contains fontfile-filename" {
  "${docxbox}" lsf "${path_docx}" | grep --count "fontTable.xml"
}

@test "Output of \"${base_command}\" contains amount fonts" {
  "${docxbox}" lsf "${path_docx}" | grep --count "8 fonts"
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
    "${docxbox}" lsf "${path_docx}" | grep --count "${i}"
  done
}

@test "Output of \"${base_command}\" can contain alternative font names" {
  "${docxbox}" lsf "${path_docx}" | grep --count "ＭＳ 明朝"
}

@test "Output of \"${base_command}\" contains font-charSets" {
  "${docxbox}" lsf "${path_docx}" | grep --count "00"
}

@test "Output of \"${base_command}\" contains font-family" {
  font_family=(
  "roman"
  "swiss"
  "auto")

  for i in "${font_family[@]}"
  do
    "${docxbox}" lsf "${path_docx}" | grep --count "${i}"
  done
}

@test "Output of \"${base_command}\" contains font-pitch" {
  "${docxbox}" lsf "${path_docx}" | grep --count "variable"
  "${docxbox}" lsf "${path_docx}" | grep --count "fixed"
}

@test "Output of \"docxbox lsf nonexistent.docx\" is an error message" {
  err_log="test/functional/tmp/err.log"

  run "$BATS_TEST_DIRNAME"/docxbox lsf nonexistent.docx
  [ "$status" -ne 0 ]

  "$BATS_TEST_DIRNAME"/docxbox lsf nonexistent.docx 2>&1 | tee "${err_log}"
  cat "${err_log}" | grep --count "docxBox Error - File not found:"
}

@test "Output of \"docxbox lsf wrong_file_type\" is an error message" {
  err_log="test/functional/tmp/err.log"
  wrong_file_types=(
  "test/functional/tmp/cp_lorem_ipsum.pdf"
  "test/functional/tmp/cp_mock_csv.csv"
  "test/functional/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    "${docxbox}" lsf "${i}" 2>&1 | tee "${err_log}"
    cat "${err_log}" | grep --count "docxBox Error - File is no ZIP archive:"
  done
}
