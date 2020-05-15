#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

@test "Output of \"docxbox rmt {missing filename}\" is an error message" {
  run "$BATS_TEST_DIRNAME"/docxbox rmt
  [ "$status" -ne 0 ]
  [ "Missing argument: DOCX filename" = "${lines[0]}" ]
}

@test "Output of \"docxbox rmt filename.docx {missing arguments}\" is an error message" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  run "$BATS_TEST_DIRNAME"/docxbox rmt "${path_docx}"
  [ "$status" -ne 0 ]
  [ "Missing argument: String left-hand-side of part to be removed" = "${lines[0]}" ]
}

#@test "With \"docxbox rem filename.docx leftHandString rightHandString\" removes text between and including given strings" {
#  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"
#
#  "$BATS_TEST_DIRNAME"/docxbox rem $path_docx Dolore incididunt
#  "$BATS_TEST_DIRNAME"/docxbox txt $path_docx | grep -vc "Dolore labore in dolor incididunt"
#  "$BATS_TEST_DIRNAME"/docxbox txt $path_docx | grep -c "Officia veniam, tempor irure lorem"
#  "$BATS_TEST_DIRNAME"/docxbox txt $path_docx | grep -c "Velit sint aute deserunt laboris"
#}
