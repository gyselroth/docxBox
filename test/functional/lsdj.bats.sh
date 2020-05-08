#!/usr/bin/env bats

load _helper

@test "Exit code of \"docxbox lsdj filename.docx\" is zero" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  run "$BATS_TEST_DIRNAME"/docxbox lsdj $path_docx
  [ "$status" -eq 0 ]
}

@test "Output of \"docxbox lsdj {missing argument}\" is an error message" {
  run "$BATS_TEST_DIRNAME"/docxbox lsdj
  [ "$status" -ne 0 ]
  [ "Missing argument: Filename of DOCX to be extracted" = "${lines[0]}" ]
}

@test "With \"docxbox lsdj filename.docx\" the fields in the docx are listed as JSON" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  "$BATS_TEST_DIRNAME/"docxbox lsdj $path_docx | grep -c "table_unordered_list_images.docx-"
  "$BATS_TEST_DIRNAME/"docxbox lsdj $path_docx | grep -c "/word/document.xml"
}

@test "With \"docxbox ls filename.docx --fields --json\" the fields in the docx are listed as JSON" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  "$BATS_TEST_DIRNAME/"docxbox ls $path_docx --fields --json | grep -c "table_unordered_list_images.docx-"
  "$BATS_TEST_DIRNAME/"docxbox ls $path_docx --fields --json | grep -c "/word/document.xml"
}

@test "With \"docxbox ls filename.docx -dj\" the fields in the docx are listed as JSON" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  "$BATS_TEST_DIRNAME/"docxbox ls $path_docx -dj | grep -c "table_unordered_list_images.docx-"
  "$BATS_TEST_DIRNAME/"docxbox ls $path_docx -dj | grep -c "/word/document.xml"
}

@test "With \"docxbox lsd filename.docx --json\" the fields in the docx are listed as JSON" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  "$BATS_TEST_DIRNAME/"docxbox lsd $path_docx --json | grep -c "table_unordered_list_images.docx-"
  "$BATS_TEST_DIRNAME/"docxbox lsd $path_docx --json | grep -c "/word/document.xml"
}
