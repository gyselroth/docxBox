#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

docxbox=""$BATS_TEST_DIRNAME"/docxbox"

@test "Output of \"docxbox rmt {missing filename}\" is an error message" {
  run "${docxbox}" rmt
  [ "$status" -ne 0 ]
  [ "Missing argument: DOCX filename" = "${lines[0]}" ]
}

title="Output of \"docxbox rmt filename.docx {missing arguments}\" "
title+="is an error message"
@test "${title}" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"
  pattern="Missing argument: String left-hand-side of part to be removed"

  run "${docxbox}" rmt "${path_docx}"
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]
}

#@test "With \"docxbox rem filename.docx leftHandString rightHandString\" removes text between and including given strings" {
#  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"
#
#  "${docxbox}" rem $path_docx Dolore incididunt
#  "${docxbox}" txt $path_docx | grep -vc "Dolore labore in dolor incididunt"
#  "${docxbox}" txt $path_docx | grep -c "Officia veniam, tempor irure lorem"
#  "${docxbox}" txt $path_docx | grep -c "Velit sint aute deserunt laboris"
#}
