#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

base_command="docxbox lsij filename.docx"
description="are contained images as JSON"

@test "Exit code of \"${base_command}\" is zero" {
  run "$BATS_TEST_DIRNAME"/docxbox lsij "${path_docx}"
  [ "$status" -eq 0 ]
}

@test "Output of \"docxbox lsij {missing argument}\" is an error message" {
  run "$BATS_TEST_DIRNAME"/docxbox lsij
  [ "$status" -ne 0 ]
  [ "Missing argument: DOCX filename" = "${lines[0]}" ]
}

@test "Output of \"${base_command}\" is contained images as JSON" {
  "$BATS_TEST_DIRNAME"/docxbox lsij "${path_docx}" | grep -c "image1.jpeg"
}

@test "Output of \"docxbox lsi filename.docx --json\" ${description}" {
  "$BATS_TEST_DIRNAME"/docxbox lsi "${path_docx}" --json | grep -c "image1.jpeg"
}

@test "Output of \"docxbox lsi filename.docx -j\" ${description}" {
  "$BATS_TEST_DIRNAME"/docxbox lsi "${path_docx}" -j | grep -c "image1.jpeg"
}

@test "Output of \"docxbox ls filename.docx -ij\" ${description}" {
  "$BATS_TEST_DIRNAME"/docxbox lsi "${path_docx}" -ij | grep -c "image1.jpeg"
}

@test "Output of \"docxbox ls filename.docx --images --json\" ${description}" {
  "$BATS_TEST_DIRNAME"/docxbox ls "${path_docx}" --images --json | grep "image1.jpeg"
}
