#!/usr/bin/env bats
# shellcheck disable=SC1008

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

base_command="docxbox diff filename.docx"
error_message="is an error message"

@test "Output of \"docxbox diff {missing argument}\" ${error_message}" {
  run "$BATS_TEST_DIRNAME"/docxbox diff
  [ "$status" -ne 0 ]
  [ "Missing argument: DOCX file to compare with" = "${lines[0]}" ]
}

@test "Output of \"${base_command} {missing argument}\" ${error_message}" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  run "$BATS_TEST_DIRNAME"/docxbox diff "${path_docx}"
  [ "$status" -ne 0 ]
  [ "Missing argument: DOCX file to compare with" = "${lines[0]}" ]
}

title="Output of \"${base_command} otherFilename.docx {missing argument}\""
@test "${title} ${error_message}" {
  path_docx_1="test/functional/tmp/cp_table_unordered_list_images.docx"
  path_docx_2="test/files/docx/table_unordered_list_images.docx"

  pattern="Missing argument: File within DOCX archives to be compared"

  run "$BATS_TEST_DIRNAME"/docxbox diff "${path_docx_1}" "${path_docx_2}"
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]
}
