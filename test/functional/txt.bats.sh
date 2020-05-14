#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

base_command="docxbox txt filename.docx"

@test "Output of \"docxbox txt {missing argument}\" is an error message" {
  run "$BATS_TEST_DIRNAME"/docxbox txt
  [ "$status" -ne 0 ]
  [ "Missing argument: Filename of DOCX to be extracted" = "${lines[0]}" ]
}

@test "Output of \"${base_command}\" is the the plain text from given file" {
  "$BATS_TEST_DIRNAME"/docxbox txt $path_docx | grep -c "Officia"
}

appendix="is the segmented plain text from given file"
@test "Output of \"${base_command} -s \" ${appendix}" {
  "$BATS_TEST_DIRNAME"/docxbox txt $path_docx -s | wc -l | grep -c "105"
  "$BATS_TEST_DIRNAME"/docxbox txt $path_docx | wc -l | grep -c "65"
}

appendix="is the segmented plain text from given file"
@test "Output of \"${base_command} --segments \" ${appendix}" {
  "$BATS_TEST_DIRNAME"/docxbox txt $path_docx --segments | wc -l | grep -c "105"
  "$BATS_TEST_DIRNAME"/docxbox txt $path_docx | wc -l | grep -c "65"
}
