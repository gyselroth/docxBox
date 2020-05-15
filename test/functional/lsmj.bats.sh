#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

docxbox=""$BATS_TEST_DIRNAME"/docxbox"
path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

base_command="docxbox lsmj filename.docx"

description="contains information about the creation time and date"

pattern="\"created\": \"2017-08-02T11:09:18Z\""

@test "Exit code of \"${base_command}\" is zero" {
  run "${docxbox}" lsmj "${path_docx}"
  [ "$status" -eq 0 ]
}

@test "Output of \"docxbox lsmj {missing argument}\" is an error message" {
  run "${docxbox}" lsmj
  [ "$status" -ne 0 ]
  [ "Missing argument: Filename of DOCX to be extracted" = "${lines[0]}" ]
}

@test "Output of \"${base_command}\" contains information about the xml schema" {
  pattern="\"xmlSchema\": \"http://schemas.openxmlformats.org/officeDocument/2006\""

  "${docxbox}" lsmj "${path_docx}" | grep --count "${pattern}"
}

@test "Output of \"docxbox lsm filename.docx --json\" ${description}" {
  "${docxbox}" lsm "${path_docx}" --json | grep --count "${pattern}"
}

@test "Output of \"docxbox lsm filename.docx -j\" ${description}" {
  "${docxbox}" lsm "${path_docx}" -j | grep --count "${pattern}"
}

@test "Output of \"docxbox ls filename.docx --meta --json\" ${description}" {
  "${docxbox}" ls "${path_docx}" --meta --json | grep --count "${pattern}"
}

@test "Output of \"docxbox ls filename.docx -mj\" ${description}" {
  "${docxbox}" ls "${path_docx}" -mj | grep --count "${pattern}"
}

@test "Output of \"${base_command}\" contains language information" {
  pattern="\"language\": \"en-US\""

  "${docxbox}" lsmj "${path_docx}" | grep --count "${pattern}"
}

@test "Output of \"${base_command}\" contains information about the revision" {
  "${docxbox}" lsmj "${path_docx}" | grep --count "\"revision\": \"0\""
}
