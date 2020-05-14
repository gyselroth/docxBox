#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"
base_command="docxbox lsf filename.docx"
longhand_command="docxbox ls filename.docx"

@test "Exit code of \"${base_command}\" is zero" {
  run "$BATS_TEST_DIRNAME"/docxbox lsf $path_docx
  [ "$status" -eq 0 ]
}

@test "Output of \"docxbox lsf {missing argument}\" is an error message" {
  run "$BATS_TEST_DIRNAME"/docxbox lsf
  [ "$status" -ne 0 ]
  [ "Missing argument: Filename of DOCX to be extracted" = "${lines[0]}" ]
}

@test "Output of \"${base_command}\" contains ground informations" {
  run "$BATS_TEST_DIRNAME"/docxbox lsf $path_docx
  [ "$status" -eq 0 ]
  [ "word/fontTable.xml lists 12 fonts:" = "${lines[0]}" ]
}

@test "Output of \"${longhand_command} --fonts\" contains ground informations" {
  run "$BATS_TEST_DIRNAME"/docxbox ls $path_docx --fonts
  [ "$status" -eq 0 ]
  [ "word/fontTable.xml lists 12 fonts:" = "${lines[0]}" ]
}

@test "Output of \"${longhand_command} -f\" contains ground informations" {
  run "$BATS_TEST_DIRNAME"/docxbox ls $path_docx -f
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

  for i in ${attributes[@]}
  do
    "$BATS_TEST_DIRNAME"/docxbox lsf $path_docx | grep -c $i
  done
}

@test "Output of \"${base_command}\" contains fontfile-filename" {
  "$BATS_TEST_DIRNAME"/docxbox lsf $path_docx | grep -c "fontTable.xml"
}

@test "Output of \"${base_command}\" contains amount fonts" {
  "$BATS_TEST_DIRNAME"/docxbox lsf $path_docx | grep -c "12 fonts"
}

@test "Output of \"${base_command}\" contains font names" {
  font_names=(
  "Times New Roman"
  "Symbol"
  "Arial"
  "Liberation Serif")

  for i in ${font_names[@]}
  do
    "$BATS_TEST_DIRNAME"/docxbox lsf $path_docx | grep -c $i
  done
}

@test "Output of \"${base_command}\" can contain alternative font names" {
  "$BATS_TEST_DIRNAME"/docxbox lsf $path_docx | grep -c "Arial Unicode MS"
}

@test "Output of \"${base_command}\" contains font-charSets" {
  "$BATS_TEST_DIRNAME"/docxbox lsf $path_docx | grep -c "00"
}

@test "Output of \"${base_command}\" contains font-family" {
  font_family=(
  "roman"
  "swiss"
  "auto")

  for i in ${font_family[@]}
  do
    "$BATS_TEST_DIRNAME"/docxbox lsf $path_docx | grep -c $i
  done
}

@test "Output of \"${base_command}\" contains font-pitch" {
  "$BATS_TEST_DIRNAME"/docxbox lsf $path_docx | grep -c "variable"
  "$BATS_TEST_DIRNAME"/docxbox lsf $path_docx | grep -c "default"
}
