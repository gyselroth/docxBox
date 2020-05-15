#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

base_command="docxbox lsmj filename.docx"

description="contains information about the creation time and date"

pattern="\"created\": \"2017-08-02T11:09:18Z\""

@test "Exit code of \"${base_command}\" is zero" {
  run "$BATS_TEST_DIRNAME"/docxbox lsmj "${path_docx}"
  [ "$status" -eq 0 ]
}

@test "Output of \"docxbox lsmj {missing argument}\" is an error message" {
  run "$BATS_TEST_DIRNAME"/docxbox lsmj
  [ "$status" -ne 0 ]
  [ "Missing argument: Filename of DOCX to be extracted" = "${lines[0]}" ]
}

@test "Output of \"${base_command}\" contains information about the xml schema" {
  pattern="\"xmlSchema\": \"http://schemas.openxmlformats.org/officeDocument/2006\""

  "$BATS_TEST_DIRNAME"/docxbox lsmj "${path_docx}" | grep -c "${pattern}"
}

@test "Output of \"docxbox lsm filename.docx --json\" ${description}" {
  "$BATS_TEST_DIRNAME"/docxbox lsm "${path_docx}" --json | grep -c "${pattern}"
}

@test "Output of \"docxbox lsm filename.docx -j\" ${description}" {
  "$BATS_TEST_DIRNAME"/docxbox lsm "${path_docx}" -j | grep -c "${pattern}"
}

@test "Output of \"docxbox ls filename.docx --meta --json\" ${description}" {
  "$BATS_TEST_DIRNAME"/docxbox ls "${path_docx}" --meta --json | grep -c "${pattern}"
}

@test "Output of \"docxbox ls filename.docx -mj\" ${description}" {
  "$BATS_TEST_DIRNAME"/docxbox ls "${path_docx}" -mj | grep -c "${pattern}"
}

@test "Output of \"${base_command}\" contains language information" {
  pattern="\"language\": \"en-US\""

  "$BATS_TEST_DIRNAME"/docxbox lsmj "${path_docx}" | grep -c "${pattern}"
}

@test "Output of \"${base_command}\" contains information about the revision" {
  "$BATS_TEST_DIRNAME"/docxbox lsmj "${path_docx}" | grep -c "\"revision\": \"0\""
}
