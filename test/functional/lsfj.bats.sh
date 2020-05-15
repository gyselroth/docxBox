#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

base_command="docxbox lsfj filename.docx"
longhand_command="docxbox lsf filename.docx"

long_description="contains files' and directories' attributes"

attributes=(
  "font"
  "altName"
  "charset"
  "family"
  "pitch")

@test "Exit code of \"docxbox ls filename.docx\" is zero" {
  run "$BATS_TEST_DIRNAME"/docxbox lsfj "${path_docx}"
  [ "$status" -eq 0 ]
}

@test "Output of \"docxbox lsfj {missing argument}\" is an error message" {
  run "$BATS_TEST_DIRNAME"/docxbox lsfj
  [ "$status" -ne 0 ]
  [ "Missing argument: Filename of DOCX to be extracted" = "${lines[0]}" ]
}

@test "Output of \"${base_command}\" ${long_description}" {
  for i in "${attributes[@]}"
  do
    "$BATS_TEST_DIRNAME"/docxbox lsfj "${path_docx}" | grep -c "${i}"
  done
}

@test "Output of \"${longhand_command} --json\" ${long_description}" {
  for i in "${attributes[@]}"
  do
    "$BATS_TEST_DIRNAME"/docxbox lsf "${path_docx}" --json | grep -c "${i}"
  done
}

@test "Output of \"${longhand_command} -j\" ${long_description}" {
  for i in "${attributes[@]}"
  do
    "$BATS_TEST_DIRNAME"/docxbox lsf "${path_docx}" -j | grep -c "${i}"
  done
}

longhand="--fonts --json"
@test "Output of \"docxbox ls filename.docx ${longhand}\" ${long_description}" {
  for i in "${attributes[@]}"
  do
    "$BATS_TEST_DIRNAME"/docxbox ls "${path_docx}" ${longhand} | grep -c "${i}"
  done
}

@test "Output of \"docxbox ls filename.docx -fj\" ${long_description}" {
  for i in "${attributes[@]}"
  do
    "$BATS_TEST_DIRNAME"/docxbox ls "${path_docx}" -fj | grep -c "${i}"
  done
}

@test "Output of \"${base_command}\" contains fontfile-filename" {
  "$BATS_TEST_DIRNAME"/docxbox lsfj "${path_docx}" | grep -c "fontTable.xml"
}

@test "Output of \"${base_command}\" contains font names" {
  font_names=(
  "Times New Roman"
  "Symbol"
  "Arial"
  "Liberation Serif"
  "OpenSymbol"
  "Liberation Sans"
  "DejaVu Sans"
  "OpenSymbol")

  for i in "${font_names[@]}"
  do
    "$BATS_TEST_DIRNAME"/docxbox lsfj "${path_docx}" | grep -c "${i}"
  done
}

@test "Output of \"${base_command}\" can contain alternative font names" {
  "$BATS_TEST_DIRNAME"/docxbox lsfj "${path_docx}" | grep -c "Arial Unicode MS"
}

@test "Output of \"${base_command}\" contains font-charSets" {
  "$BATS_TEST_DIRNAME"/docxbox lsfj "${path_docx}" | grep -c "00"
}

@test "Output of \"${base_command}\" contains font-family" {
  font_family=(
  "roman"
  "swiss"
  "auto")

  for i in "${font_family[@]}"
  do
    "$BATS_TEST_DIRNAME"/docxbox lsfj "${path_docx}" | grep -c "${i}"
  done
}

@test "Output of \"${base_command}\" contains font-pitch" {
  "$BATS_TEST_DIRNAME"/docxbox lsfj "${path_docx}" | grep -c "variable"
  "$BATS_TEST_DIRNAME"/docxbox lsfj "${path_docx}" | grep -c "default"
}
