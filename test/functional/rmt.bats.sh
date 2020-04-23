#!/usr/bin/env bats

load _helper

@test "Output of \"docxbox rmt {missing filename}\" is an error message" {
  run "$BATS_TEST_DIRNAME"/docxbox rmt
  [ "$status" -ne 0 ]
  [ "Missing argument: DOCX Filename" = "${lines[0]}" ]
}

@test "Output of \"docxbox rmt filename.docx {missing arguments}\" is an error message" {
  run "$BATS_TEST_DIRNAME"/docxbox rmt test/functional/tmp/cp_table_unordered_list_images.docx
  [ "$status" -ne 0 ]
  [ "Missing argument: String left-hand-side of part to be removed" = "${lines[0]}" ]
}

#@test "With \"docxbox rem filename.docx leftHandString rightHandString\" removes text between and including given strings" {
#  "$BATS_TEST_DIRNAME"/docxbox rem test/functional/tmp/cp_table_unordered_list_images.docx Dolore incididunt
#  "$BATS_TEST_DIRNAME"/docxbox txt test/functional/tmp/cp_table_unordered_list_images.docx | grep -vc "Dolore labore in dolor incididunt"
#  "$BATS_TEST_DIRNAME"/docxbox txt test/functional/tmp/cp_table_unordered_list_images.docx | grep -c "Officia veniam, tempor irure lorem"
#  "$BATS_TEST_DIRNAME"/docxbox txt test/functional/tmp/cp_table_unordered_list_images.docx | grep -c "Velit sint aute deserunt laboris"
#}
