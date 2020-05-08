#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

@test "Exit code of \"docxbox lsl filename.docx searchString\" is zero" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"
  
  run "$BATS_TEST_DIRNAME"/docxbox lsl $path_docx fonts
  [ "$status" -eq 0 ]
}

@test "Output of \"docxbox lsl {missing argument}\" is an error message" {
  run "$BATS_TEST_DIRNAME"/docxbox lsl
  [ "$status" -ne 0 ]
  [ "Missing argument: DOCX filename" = "${lines[0]}" ]
}

@test "Output of \"docxbox lsl filename.docx {missing argument}\" is an error message" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  run "$BATS_TEST_DIRNAME"/docxbox lsl path_docx
  [ "$status" -ne 0 ]
  [ "Missing argument: String or regular expression to be located" = "${lines[0]}" ]
}

@test "\"docxbox lsl filename.docx searchString\" lists all files containing given searchString" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  "$BATS_TEST_DIRNAME"/docxbox lsl $path_docx fonts | grep -c "word/document.xml"
  "$BATS_TEST_DIRNAME"/docxbox lsl $path_docx fonts | grep -c "word/fontTable.xml"
  "$BATS_TEST_DIRNAME"/docxbox lsl $path_docx fonts | grep -c "word/numbering.xml"
  "$BATS_TEST_DIRNAME"/docxbox lsl $path_docx fonts | grep -c "word/styles.xml"
}

@test "\"docxbox ls filename.docx -l searchString\" lists all files containing given searchString" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  "$BATS_TEST_DIRNAME"/docxbox ls $path_docx -l fonts | grep -c "word/document.xml"
  "$BATS_TEST_DIRNAME"/docxbox ls $path_docx -l fonts | grep -c "word/fontTable.xml"
  "$BATS_TEST_DIRNAME"/docxbox ls $path_docx -l fonts | grep -c "word/numbering.xml"
  "$BATS_TEST_DIRNAME"/docxbox ls $path_docx -l fonts | grep -c "word/styles.xml"
}

@test "\"docxbox ls filename.docx --locate searchString\" lists all files containing given searchString" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  "$BATS_TEST_DIRNAME"/docxbox ls $path_docx --locate fonts | grep -c "word/document.xml"
  "$BATS_TEST_DIRNAME"/docxbox ls $path_docx --locate fonts | grep -c "word/fontTable.xml"
  "$BATS_TEST_DIRNAME"/docxbox ls $path_docx --locate fonts | grep -c "word/numbering.xml"
  "$BATS_TEST_DIRNAME"/docxbox ls $path_docx --locate fonts | grep -c "word/styles.xml"
}

@test "\"docxbox lsl filename.docx regular-expression\" lists all files containing given regular-expression" {
  path_docx="test/functional/tmp/cp_bio_assay.docx"

  "$BATS_TEST_DIRNAME"/docxbox lsl $path_docx "/[0-9A-Z]\{8\}/" | grep -c "docProps/core.xml"
}

@test "\"docxbox ls filename.docx -l regular-expression\" lists all files containing given regular-expression" {
  path_docx="test/functional/tmp/cp_bio_assay.docx"

  "$BATS_TEST_DIRNAME"/docxbox ls $path_docx -l "/[0-9A-Z]\{8\}/" | grep -c "docProps/core.xml"
}

@test "\"docxbox ls filename.docx --locate regular-expression\" lists all files containing given regular-expression" {
  path_docx="test/functional/tmp/cp_bio_assay.docx"

  "$BATS_TEST_DIRNAME"/docxbox ls $path_docx --locate "/[0-9A-Z]\{8\}/" | grep -c "docProps/core.xml"
}
