#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

docxbox=""$BATS_TEST_DIRNAME"/docxbox"
path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

base_command="docxbox lsf filename.docx"
longhand_command="docxbox ls filename.docx"

@test "Exit code of \"${base_command}\" is zero" {
  run "${docxbox}" lsf "${path_docx}"
  [ "$status" -eq 0 ]
}

@test "Output of \"docxbox lsf {missing argument}\" is an error message" {
  run "${docxbox}" lsf
  [ "$status" -ne 0 ]
  [ "Missing argument: Filename of DOCX to be extracted" = "${lines[0]}" ]
}

@test "Output of \"${base_command}\" contains ground informations" {
  run "${docxbox}" lsf "${path_docx}"
  [ "$status" -eq 0 ]
  [ "word/fontTable.xml lists 12 fonts:" = "${lines[0]}" ]
}

@test "Output of \"${longhand_command} --fonts\" contains ground informations" {
  run "${docxbox}" ls "${path_docx}" --fonts
  [ "$status" -eq 0 ]
  [ "word/fontTable.xml lists 12 fonts:" = "${lines[0]}" ]
}

@test "Output of \"${longhand_command} -f\" contains ground informations" {
  run "${docxbox}" ls "${path_docx}" -f
  [ "$status" -eq 0 ]
  [ "word/fontTable.xml lists 12 fonts:" = "${lines[0]}" ]
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
  "${docxbox}" lsf "${path_docx}" | grep --count "12 fonts"
}

@test "Output of \"${base_command}\" contains font names" {
  font_names=(
  "Times New Roman"
  "Symbol"
  "Arial"
  "Liberation Serif")

  for i in "${font_names[@]}"
  do
    "${docxbox}" lsf "${path_docx}" | grep --count "${i}"
  done
}

@test "Output of \"${base_command}\" can contain alternative font names" {
  "${docxbox}" lsf "${path_docx}" | grep --count "Arial Unicode MS"
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
  "${docxbox}" lsf "${path_docx}" | grep --count "default"
}
