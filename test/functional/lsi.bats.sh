#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"
base_command="docxbox lsi filename.docx"

@test "Exit code of \"${base_command}\" is zero" {
  run "$BATS_TEST_DIRNAME"/docxbox lsi $path_docx
  [ "$status" -eq 0 ]
}

@test "Output of \"docxbox lsi {missing argument}\" is an error message" {
  run "$BATS_TEST_DIRNAME"/docxbox lsi
  [ "$status" -ne 0 ]
  [ "Missing argument: Filename of DOCX to be extracted" = "${lines[0]}" ]
}

@test "Output of ${base_command}\" contains files' and directories' attributes" {
  attributes=(
  "Length"
  "Date"
  "Time"
  "Name")

  for i in ${attributes[@]}
  do
    "$BATS_TEST_DIRNAME"/docxbox ls $path_docx | grep -c $i
  done
}

@test "Output of \"${base_command}\" is contained images" {
  run "$BATS_TEST_DIRNAME"/docxbox lsi $path_docx
  [ "$status" -eq 0 ]
  "$BATS_TEST_DIRNAME"/docxbox lsi $path_docx | grep -c image1.jpeg
}

@test "Output of \"docxbox ls filename.docx -i\" is contained images" {
  "$BATS_TEST_DIRNAME"/docxbox ls $path_docx -i | grep -c image1.jpeg
}

@test "Output of \"docxbox ls filename.docx --images\" is contained images" {
  "$BATS_TEST_DIRNAME"/docxbox ls $path_docx --images | grep -c image1.jpeg
}
