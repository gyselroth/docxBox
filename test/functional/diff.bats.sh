#!/usr/bin/env bats
# shellcheck disable=SC1008

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

docxbox=""$BATS_TEST_DIRNAME"/docxbox"

path_docx_1="test/functional/tmp/cp_table_unordered_list_images.docx"
path_docx_2="test/functional/tmp/cp_bio_assay.docx"

base_command="docxbox diff filename.docx"
error_message="is an error message"

@test "Output of \"docxbox diff {missing argument}\" ${error_message}" {
  run "${docxbox}" diff
  [ "$status" -ne 0 ]
  [ "Missing argument: DOCX file to compare with" = "${lines[0]}" ]
}

@test "Output of \"${base_command} {missing argument}\" ${error_message}" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  run "${docxbox}" diff "${path_docx}"
  [ "$status" -ne 0 ]
  [ "Missing argument: DOCX file to compare with" = "${lines[0]}" ]
}

title="Output of \"${base_command} otherFilename.docx {missing argument}\""
@test "${title} ${error_message}" {
  pattern="Missing argument: File within DOCX archives to be compared"

  run "${docxbox}" diff "${path_docx_1}" "${path_docx_2}"
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]
}

description="a side by side view is displayed"
@test "With \"${base_command} changedFilename.docx\" ${description}" {
  path_changed_docx="test/functional/tmp/cp_table_unordered_list_images_v2.docx"
  
  run "${docxbox}" lorem "${path_docx_1}" "${path_changed_docx}"
  
  "${docxbox}" diff "${path_docx_1}" "${path_changed_docx}" word/document.xml | grep "|"
}
