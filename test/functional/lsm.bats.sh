#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

base_command="docxbox lsm filename.docx"

@test "Exit code of \"${base_command}\" is zero" {
  run "$BATS_TEST_DIRNAME"/docxbox lsm $path_docx
  [ "$status" -eq 0 ]
}

@test "Output of \"docxbox lsm {missing argument}\" is an error message" {
  run "$BATS_TEST_DIRNAME"/docxbox lsm
  [ "$status" -ne 0 ]
  [ "Missing argument: Filename of DOCX to be extracted" = "${lines[0]}" ]
}

@test "Output of \"${base_command}\" contains information about the xml schema" {
  xml_schema="xmlSchema: http://schemas.openxmlformats.org/officeDocument/2006"

  run "$BATS_TEST_DIRNAME"/docxbox lsm $path_docx
  [ "$status" -eq 0 ]
  [ "$xml_schema" = "${lines[0]}" ]
}

title="Output of \"${base_command}\" "
title+="contains information about the creation time and date"
@test "${title}" {
  run "$BATS_TEST_DIRNAME"/docxbox lsm $path_docx
  [ "$status" -eq 0 ]
  [ "created: 2017-08-02T11:09:18Z" = "${lines[1]}" ]
}

@test "Output of \"${base_command}\" contains language information" {
  run "$BATS_TEST_DIRNAME"/docxbox lsm $path_docx
  [ "$status" -eq 0 ]
  [ "language: en-US" = "${lines[2]}" ]
}

@test "Output of \"${base_command}\" contains information about the revision" {
  run "$BATS_TEST_DIRNAME"/docxbox lsm $path_docx
  [ "$status" -eq 0 ]
  [ "revision: 0" = "${lines[3]}" ]
}
