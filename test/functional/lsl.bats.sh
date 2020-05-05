#!/usr/bin/env bats

load _helper

@test "Exit code of \"docxbox lsl filename.docx searchString\" is zero" {
  run "$BATS_TEST_DIRNAME"/docxbox lsl test/functional/tmp/cp_table_unordered_list_images.docx fonts
  [ "$status" -eq 0 ]
}

@test "Output of \"docxbox lsl {missing argument}\" is an error message" {
  run "$BATS_TEST_DIRNAME"/docxbox lsl
  [ "$status" -ne 0 ]
  [ "Missing argument: DOCX filename" = "${lines[0]}" ]
}

# @TODO: throws an error: related github issue: #22
#@test "Output of \"docxbox lsl filename.docx {missing argument}\" is an error message" {
#  run "$BATS_TEST_DIRNAME"/docxbox lsl
#  [ "$status" -ne 0 ]
#  [ "Missing argument: DOCX filename" = "${lines[0]}" ]
#}

@test "\"docxbox lsl filename.docx searchString\" list all files containing given searchString" {
  "$BATS_TEST_DIRNAME"/docxbox lsl test/functional/tmp/cp_table_unordered_list_images.docx fonts | grep -c "word/document.xml"
  "$BATS_TEST_DIRNAME"/docxbox lsl test/functional/tmp/cp_table_unordered_list_images.docx fonts | grep -c "word/fontTable.xml"
  "$BATS_TEST_DIRNAME"/docxbox lsl test/functional/tmp/cp_table_unordered_list_images.docx fonts | grep -c "word/numbering.xml"
  "$BATS_TEST_DIRNAME"/docxbox lsl test/functional/tmp/cp_table_unordered_list_images.docx fonts | grep -c "word/styles.xml"
}
