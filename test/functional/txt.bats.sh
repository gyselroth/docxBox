#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

docxbox=""$BATS_TEST_DIRNAME"/docxbox"
path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

base_command="docxbox txt filename.docx"

@test "Output of \"docxbox txt {missing argument}\" is an error message" {
  run "${docxbox}" txt
  [ "$status" -ne 0 ]
  [ "Missing argument: Filename of DOCX to be extracted" = "${lines[0]}" ]
}

@test "Output of \"${base_command}\" is the the plain text from given file" {
  "${docxbox}" txt "${path_docx}" | grep --count "Officia"
}

appendix="is the segmented plain text from given file"
@test "Output of \"${base_command} -s \" ${appendix}" {
  "${docxbox}" txt "${path_docx}" -s | wc --lines | grep --count "105"
  "${docxbox}" txt "${path_docx}" | wc --lines | grep --count "65"
}

appendix="is the segmented plain text from given file"
@test "Output of \"${base_command} --segments \" ${appendix}" {
  "${docxbox}" txt "${path_docx}" --segments | wc --lines | grep --count "105"
  "${docxbox}" txt "${path_docx}" | wc --lines | grep --count "65"
}
