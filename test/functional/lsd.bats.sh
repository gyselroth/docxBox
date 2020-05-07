#!/usr/bin/env bats

load _helper

@test "Exit code of \"docxbox lsd filename.docx\" is zero" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  run "$BATS_TEST_DIRNAME"/docxbox lsd $path_docx
  [ "$status" -eq 0 ]
}

@test "Output of \"docxbox lsd {missing argument}\" is an error message" {
  run "$BATS_TEST_DIRNAME"/docxbox lsd
  [ "$status" -ne 0 ]
  [ "Missing argument: Filename of DOCX to be extracted" = "${lines[0]}" ]
}

@test "With \"docxbox lsd filename.docx\" the fields in the docx are listed" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  "$BATS_TEST_DIRNAME/"docxbox lsd $path_docx | grep -c "table_unordered_list_images.docx-"
  "$BATS_TEST_DIRNAME/"docxbox lsd $path_docx | grep -c "/word/document.xml"
}

@test "With \"docxbox ls filename.docx --fields\" the fields in the docx are listed" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  "$BATS_TEST_DIRNAME/"docxbox ls $path_docx --fields | grep -c "table_unordered_list_images.docx-"
  "$BATS_TEST_DIRNAME/"docxbox ls $path_docx --fields | grep -c "/word/document.xml"
}

@test "With \"docxbox ls filename.docx --d\" the fields in the docx are listed" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  "$BATS_TEST_DIRNAME/"docxbox ls $path_docx -d | grep -c "table_unordered_list_images.docx-"
  "$BATS_TEST_DIRNAME/"docxbox ls $path_docx -d | grep -c "/word/document.xml"
}
