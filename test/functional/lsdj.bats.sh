#!/usr/bin/env bats

load _helper

@test "Exit code of \"docxbox lsdj filename.docx\" is zero" {
  run "$BATS_TEST_DIRNAME"/docxbox lsdj test/functional/tmp/cp_table_unordered_list_images.docx
  [ "$status" -eq 0 ]
}

@test "Output of \"docxbox lsdj {missing argument}\" is an error message" {
  run "$BATS_TEST_DIRNAME"/docxbox lsdj
  [ "$status" -ne 0 ]
  [ "Missing argument: Filename of DOCX to be extracted" = "${lines[0]}" ]
}

@test "With \"docxbox lsdj filename.docx\" the fields in the docx are listed as JSON" {
  "$BATS_TEST_DIRNAME/"docxbox lsdj test/functional/tmp/cp_table_unordered_list_images.docx | grep -c "table_unordered_list_images.docx-"
  "$BATS_TEST_DIRNAME/"docxbox lsdj test/functional/tmp/cp_table_unordered_list_images.docx | grep -c "/word/document.xml"
}

@test "With \"docxbox ls filename.docx --fields --json\" the fields in the docx are listed as JSON" {
  "$BATS_TEST_DIRNAME/"docxbox ls test/functional/tmp/cp_table_unordered_list_images.docx --fields --json | grep -c "table_unordered_list_images.docx-"
  "$BATS_TEST_DIRNAME/"docxbox ls test/functional/tmp/cp_table_unordered_list_images.docx --fields --json | grep -c "/word/document.xml"
}

@test "With \"docxbox ls filename.docx -dj\" the fields in the docx are listed as JSON" {
  "$BATS_TEST_DIRNAME/"docxbox ls test/functional/tmp/cp_table_unordered_list_images.docx -dj | grep -c "table_unordered_list_images.docx-"
  "$BATS_TEST_DIRNAME/"docxbox ls test/functional/tmp/cp_table_unordered_list_images.docx -dj | grep -c "/word/document.xml"
}

@test "With \"docxbox lsd filename.docx --json\" the fields in the docx are listed as JSON" {
  "$BATS_TEST_DIRNAME/"docxbox lsd test/functional/tmp/cp_table_unordered_list_images.docx --json | grep -c "table_unordered_list_images.docx-"
  "$BATS_TEST_DIRNAME/"docxbox lsd test/functional/tmp/cp_table_unordered_list_images.docx --json | grep -c "/word/document.xml"
}
