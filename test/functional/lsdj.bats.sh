#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

base_command="docxbox lsdj filename.docx"
path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"
long_description_json="the fields in the docx are listed as JSON"

@test "Exit code of \"${base_command}\" is zero" {
  run "$BATS_TEST_DIRNAME"/docxbox lsdj $path_docx
  [ "$status" -eq 0 ]
}

@test "Output of \"docxbox lsdj {missing argument}\" is an error message" {
  run "$BATS_TEST_DIRNAME"/docxbox lsdj
  [ "$status" -ne 0 ]
  [ "Missing argument: Filename of DOCX to be extracted" = "${lines[0]}" ]
}

@test "With \"${base_command}\" the fields in the docx are listed as JSON" {
  pattern="table_unordered_list_images.docx-"
  "$BATS_TEST_DIRNAME/"docxbox lsdj $path_docx | grep -c $pattern

  pattern="/word/document.xml"
  "$BATS_TEST_DIRNAME/"docxbox lsdj $path_docx | grep -c $pattern
}

title="With \"docxbox ls filename.docx --fields --json\" "
title+="${long_description_json}"
@test "${title}" {
  pattern="table_unordered_list_images.docx-"
  "$BATS_TEST_DIRNAME/"docxbox ls $path_docx --fields --json | grep -c $pattern

  pattern="/word/document.xml"
  "$BATS_TEST_DIRNAME/"docxbox ls $path_docx --fields --json | grep -c $pattern
}

@test "With \"docxbox ls filename.docx -dj\" ${long_description_json}" {
  pattern="table_unordered_list_images.docx-"
  "$BATS_TEST_DIRNAME/"docxbox ls $path_docx -dj | grep -c $pattern

  pattern="/word/document.xml"
  "$BATS_TEST_DIRNAME/"docxbox ls $path_docx -dj | grep -c $pattern
}

@test "With \"docxbox lsd filename.docx --json\" ${long_description_json}" {
  pattern="table_unordered_list_images.docx-"
  "$BATS_TEST_DIRNAME/"docxbox lsd $path_docx --json | grep -c $pattern

  pattern="/word/document.xml"
  "$BATS_TEST_DIRNAME/"docxbox lsd $path_docx --json | grep -c $pattern
}
