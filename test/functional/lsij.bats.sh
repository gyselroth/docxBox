#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

@test "Exit code of \"docxbox lsij filename.docx\" is zero" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  run "$BATS_TEST_DIRNAME"/docxbox lsij $path_docx
  [ "$status" -eq 0 ]
}

@test "Output of \"docxbox lsij {missing argument}\" is an error message" {
  run "$BATS_TEST_DIRNAME"/docxbox lsij
  [ "$status" -ne 0 ]
  [ "Missing argument: DOCX filename" = "${lines[0]}" ]
}

@test "Output of \"docxbox lsij filename.docx\" is contained images as JSON" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  "$BATS_TEST_DIRNAME"/docxbox lsij $path_docx | grep -c "image1.jpeg"
}

@test "Output of \"docxbox lsi filename.docx --json\" are contained images as JSON" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  "$BATS_TEST_DIRNAME"/docxbox lsi $path_docx --json | grep -c "image1.jpeg"
}

@test "Output of \"docxbox lsi filename.docx -j\" are contained images as JSON" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  "$BATS_TEST_DIRNAME"/docxbox lsi $path_docx -j | grep -c "image1.jpeg"
}
