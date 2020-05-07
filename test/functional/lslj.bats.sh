#!/usr/bin/env bats

load _helper

@test "Output of \"docxbox ls {missing argument}\" is an error message" {
  run "$BATS_TEST_DIRNAME"/docxbox lslj
  [ "$status" -ne 0 ]
  [ "Missing argument: String (or regular expression) to be found" = "${lines[0]}" ]
}

@test "Output of \"docxbox lslj filename.docx {missing argument} is an error message" {
  run "$BATS_TEST_DIRNAME"/docxbox lslj
  [ "$status" -ne 0 ]
  [ "Missing argument: String (or regular expression) to be found" = "${lines[0]}" ]
}

@test "\"docxbox lsl filename.docx searchString\" lists all files containing given searchString as JSON" {
  "$BATS_TEST_DIRNAME"/docxbox lslj test/functional/tmp/cp_table_unordered_list_images.docx fonts | grep -c "word/document.xml"
  "$BATS_TEST_DIRNAME"/docxbox lslj test/functional/tmp/cp_table_unordered_list_images.docx fonts | grep -c "word/fontTable.xml"
  "$BATS_TEST_DIRNAME"/docxbox lslj test/functional/tmp/cp_table_unordered_list_images.docx fonts | grep -c "word/numbering.xml"
  "$BATS_TEST_DIRNAME"/docxbox lslj test/functional/tmp/cp_table_unordered_list_images.docx fonts | grep -c "word/styles.xml"
  run "$BATS_TEST_DIRNAME"/docxbox lslj test/functional/tmp/cp_table_unordered_list_images.docx fonts
  [ "     7433  4/11/2020 11:3     word/document.xml" != "${lines[4]}" ]
}

@test "\"docxbox lsl filename.docx -j searchString\" lists all files containing given searchString as JSON" {
  "$BATS_TEST_DIRNAME"/docxbox lsl test/functional/tmp/cp_table_unordered_list_images.docx -j fonts | grep -c "word/document.xml"
  "$BATS_TEST_DIRNAME"/docxbox lsl test/functional/tmp/cp_table_unordered_list_images.docx -j fonts | grep -c "word/fontTable.xml"
  "$BATS_TEST_DIRNAME"/docxbox lsl test/functional/tmp/cp_table_unordered_list_images.docx -j fonts | grep -c "word/numbering.xml"
  "$BATS_TEST_DIRNAME"/docxbox lsl test/functional/tmp/cp_table_unordered_list_images.docx -j fonts | grep -c "word/styles.xml"
  run "$BATS_TEST_DIRNAME"/docxbox lslj test/functional/tmp/cp_table_unordered_list_images.docx fonts
  [ "     7433  4/11/2020 11:3     word/document.xml" != "${lines[4]}" ]
}

@test "\"docxbox lsl filename.docx --json searchString\" lists all files containing given searchString as JSON" {
  "$BATS_TEST_DIRNAME"/docxbox lsl test/functional/tmp/cp_table_unordered_list_images.docx --json fonts | grep -c "word/document.xml"
  "$BATS_TEST_DIRNAME"/docxbox lsl test/functional/tmp/cp_table_unordered_list_images.docx --json fonts | grep -c "word/fontTable.xml"
  "$BATS_TEST_DIRNAME"/docxbox lsl test/functional/tmp/cp_table_unordered_list_images.docx --json fonts | grep -c "word/numbering.xml"
  "$BATS_TEST_DIRNAME"/docxbox lsl test/functional/tmp/cp_table_unordered_list_images.docx --json fonts | grep -c "word/styles.xml"
  run "$BATS_TEST_DIRNAME"/docxbox lslj test/functional/tmp/cp_table_unordered_list_images.docx fonts
  [ "     7433  4/11/2020 11:3     word/document.xml" != "${lines[4]}" ]
}

@test "\"docxbox ls filename.docx --lj searchString\" lists all files containing given searchString as JSON" {
  "$BATS_TEST_DIRNAME"/docxbox ls test/functional/tmp/cp_table_unordered_list_images.docx --lj fonts | grep -c "word/document.xml"
  "$BATS_TEST_DIRNAME"/docxbox ls test/functional/tmp/cp_table_unordered_list_images.docx --lj fonts | grep -c "word/fontTable.xml"
  "$BATS_TEST_DIRNAME"/docxbox ls test/functional/tmp/cp_table_unordered_list_images.docx --lj fonts | grep -c "word/numbering.xml"
  "$BATS_TEST_DIRNAME"/docxbox ls test/functional/tmp/cp_table_unordered_list_images.docx --lj fonts | grep -c "word/styles.xml"
  run "$BATS_TEST_DIRNAME"/docxbox lslj test/functional/tmp/cp_table_unordered_list_images.docx fonts
  [ "     7433  4/11/2020 11:3     word/document.xml" != "${lines[4]}" ]
}

@test "\"docxbox ls filename.docx --locate -j searchString\" lists all files containing given searchString as JSON" {
  "$BATS_TEST_DIRNAME"/docxbox ls test/functional/tmp/cp_table_unordered_list_images.docx --locate -j fonts | grep -c "word/document.xml"
  "$BATS_TEST_DIRNAME"/docxbox ls test/functional/tmp/cp_table_unordered_list_images.docx --locate -j fonts | grep -c "word/fontTable.xml"
  "$BATS_TEST_DIRNAME"/docxbox ls test/functional/tmp/cp_table_unordered_list_images.docx --locate -j fonts | grep -c "word/numbering.xml"
  "$BATS_TEST_DIRNAME"/docxbox ls test/functional/tmp/cp_table_unordered_list_images.docx --locate -j fonts | grep -c "word/styles.xml"
  run "$BATS_TEST_DIRNAME"/docxbox lslj test/functional/tmp/cp_table_unordered_list_images.docx fonts
  [ "     7433  4/11/2020 11:3     word/document.xml" != "${lines[4]}" ]
}

@test "\"docxbox ls filename.docx --locate --json searchString\" lists all files containing given searchString as JSON" {
  "$BATS_TEST_DIRNAME"/docxbox ls test/functional/tmp/cp_table_unordered_list_images.docx --locate --json fonts | grep -c "word/document.xml"
  "$BATS_TEST_DIRNAME"/docxbox ls test/functional/tmp/cp_table_unordered_list_images.docx --locate --json fonts | grep -c "word/fontTable.xml"
  "$BATS_TEST_DIRNAME"/docxbox ls test/functional/tmp/cp_table_unordered_list_images.docx --locate --json fonts | grep -c "word/numbering.xml"
  "$BATS_TEST_DIRNAME"/docxbox ls test/functional/tmp/cp_table_unordered_list_images.docx --locate --json fonts | grep -c "word/styles.xml"
  run "$BATS_TEST_DIRNAME"/docxbox lslj test/functional/tmp/cp_table_unordered_list_images.docx fonts
  [ "     7433  4/11/2020 11:3     word/document.xml" != "${lines[4]}" ]
}


