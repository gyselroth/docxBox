#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

@test "Exit code of \"docxbox ls filename.docx\" is zero" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  run "$BATS_TEST_DIRNAME"/docxbox ls $path_docx
  [ "$status" -eq 0 ]
}

@test "Output of \"docxbox lsf {missing argument}\" is an error message" {
  run "$BATS_TEST_DIRNAME"/docxbox lsf
  [ "$status" -ne 0 ]
  [ "Missing argument: Filename of DOCX to be extracted" = "${lines[0]}" ]
}

@test "Output of \"docxbox lsf filename.docx\" contains ground informations" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  run "$BATS_TEST_DIRNAME"/docxbox lsf $path_docx
  [ "$status" -eq 0 ]
  [ "word/fontTable.xml lists 12 fonts:" = "${lines[0]}" ]
}

@test "Output of \"docxbox ls filename.docx --fonts\" contains ground informations" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  run "$BATS_TEST_DIRNAME"/docxbox ls $path_docx --fonts
  [ "$status" -eq 0 ]
  [ "word/fontTable.xml lists 12 fonts:" = "${lines[0]}" ]
}

@test "Output of \"docxbox ls filename.docx -f\" contains ground informations" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  run "$BATS_TEST_DIRNAME"/docxbox ls $path_docx -f
  [ "$status" -eq 0 ]
  [ "word/fontTable.xml lists 12 fonts:" = "${lines[0]}" ]
}

@test "Output of \"docxbox lsf filename.docx\" contains files' and directories' attributes" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  "$BATS_TEST_DIRNAME"/docxbox lsf $path_docx | grep "Font"
  "$BATS_TEST_DIRNAME"/docxbox lsf $path_docx | grep "AltName"
  "$BATS_TEST_DIRNAME"/docxbox lsf $path_docx | grep "CharSet"
  "$BATS_TEST_DIRNAME"/docxbox lsf $path_docx | grep "Family"
  "$BATS_TEST_DIRNAME"/docxbox lsf $path_docx | grep "Pitch"
}

@test "Output of \"docxbox lsf filename.docx\" contains fontfile-filename" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  "$BATS_TEST_DIRNAME"/docxbox lsf $path_docx | grep -c "fontTable.xml"
}

@test "Output of \"docxbox lsf filename.docx\" contains amount fonts" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  "$BATS_TEST_DIRNAME"/docxbox lsf $path_docx | grep -c "12 fonts"
}

@test "Output of \"docxbox lsf filename.docx\" contains font names" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  "$BATS_TEST_DIRNAME"/docxbox lsf $path_docx | grep -c "Times New Roman"
  "$BATS_TEST_DIRNAME"/docxbox lsf $path_docx | grep -c "Symbol"
  "$BATS_TEST_DIRNAME"/docxbox lsf $path_docx | grep -c "Arial"
  "$BATS_TEST_DIRNAME"/docxbox lsf $path_docx | grep -c "Liberation Serif"
}

@test "Output of \"docxbox lsf filename.docx\" can contain alternative font names" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  "$BATS_TEST_DIRNAME"/docxbox lsf $path_docx | grep -c "Arial Unicode MS"
}

@test "Output of \"docxbox lsf filename.docx\" contains font-charSets" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  "$BATS_TEST_DIRNAME"/docxbox lsf $path_docx | grep -c "00"
}

@test "Output of \"docxbox lsf filename.docx\" contains font-family" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  "$BATS_TEST_DIRNAME"/docxbox lsf $path_docx | grep -c "roman"
  "$BATS_TEST_DIRNAME"/docxbox lsf $path_docx | grep -c "swiss"
  "$BATS_TEST_DIRNAME"/docxbox lsf $path_docx | grep -c "auto"
}

@test "Output of \"docxbox lsf filename.docx\" contains font-pitch" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  "$BATS_TEST_DIRNAME"/docxbox lsf $path_docx | grep -c "variable"
  "$BATS_TEST_DIRNAME"/docxbox lsf $path_docx | grep -c "default"
}
