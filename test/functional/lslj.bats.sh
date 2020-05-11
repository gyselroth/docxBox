#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

@test "Output of \"docxbox lslj {missing argument}\" is an error message" {
  run "$BATS_TEST_DIRNAME"/docxbox lslj
  [ "$status" -ne 0 ]
  [ "Missing argument: DOCX filename" = "${lines[0]}" ]
}

@test "Output of \"docxbox lslj filename.docx {missing argument} is an error message" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  run "$BATS_TEST_DIRNAME"/docxbox lslj $path_docx
  [ "$status" -ne 0 ]
  [ "Missing argument: String or regular expression to be located" = "${lines[0]}" ]
}

@test "\"docxbox lsl filename.docx searchString\" lists all files containing given searchString as JSON" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  "$BATS_TEST_DIRNAME"/docxbox lslj $path_docx fonts | grep -c "word/document.xml"
  "$BATS_TEST_DIRNAME"/docxbox lslj $path_docx fonts | grep -c "word/fontTable.xml"
  "$BATS_TEST_DIRNAME"/docxbox lslj $path_docx fonts | grep -c "word/numbering.xml"
  "$BATS_TEST_DIRNAME"/docxbox lslj $path_docx fonts | grep -c "word/styles.xml"

  run "$BATS_TEST_DIRNAME"/docxbox lslj $path_docx fonts
  [ "     7433  4/11/2020 11:3     word/document.xml" != "${lines[4]}" ]
}

@test "\"docxbox lsl filename.docx -j searchString\" lists all files containing given searchString as JSON" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  "$BATS_TEST_DIRNAME"/docxbox lsl $path_docx -j fonts | grep -c "word/document.xml"
  "$BATS_TEST_DIRNAME"/docxbox lsl $path_docx -j fonts | grep -c "word/fontTable.xml"
  "$BATS_TEST_DIRNAME"/docxbox lsl $path_docx -j fonts | grep -c "word/numbering.xml"
  "$BATS_TEST_DIRNAME"/docxbox lsl $path_docx -j fonts | grep -c "word/styles.xml"

  run "$BATS_TEST_DIRNAME"/docxbox lsl $path_docx -j fonts
  [ "     7433  4/11/2020 11:3     word/document.xml" != "${lines[4]}" ]
}

@test "\"docxbox lsl filename.docx --json searchString\" lists all files containing given searchString as JSON" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  "$BATS_TEST_DIRNAME"/docxbox lsl $path_docx --json fonts | grep -c "word/document.xml"
  "$BATS_TEST_DIRNAME"/docxbox lsl $path_docx --json fonts | grep -c "word/fontTable.xml"
  "$BATS_TEST_DIRNAME"/docxbox lsl $path_docx --json fonts | grep -c "word/numbering.xml"
  "$BATS_TEST_DIRNAME"/docxbox lsl $path_docx --json fonts | grep -c "word/styles.xml"

  run "$BATS_TEST_DIRNAME"/docxbox lslj $path_docx fonts
  [ "     7433  4/11/2020 11:3     word/document.xml" != "${lines[4]}" ]
}

@test "\"docxbox ls filename.docx --lj searchString\" lists all files containing given searchString as JSON" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  "$BATS_TEST_DIRNAME"/docxbox ls $path_docx --lj fonts | grep -c "word/document.xml"
  "$BATS_TEST_DIRNAME"/docxbox ls $path_docx --lj fonts | grep -c "word/fontTable.xml"
  "$BATS_TEST_DIRNAME"/docxbox ls $path_docx --lj fonts | grep -c "word/numbering.xml"
  "$BATS_TEST_DIRNAME"/docxbox ls $path_docx --lj fonts | grep -c "word/styles.xml"

  run "$BATS_TEST_DIRNAME"/docxbox lslj $path_docx fonts
  [ "     7433  4/11/2020 11:3     word/document.xml" != "${lines[4]}" ]
}

@test "\"docxbox ls filename.docx --locate -j searchString\" lists all files containing given searchString as JSON" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  "$BATS_TEST_DIRNAME"/docxbox ls $path_docx --locate -j fonts | grep -c "word/document.xml"
  "$BATS_TEST_DIRNAME"/docxbox ls $path_docx --locate -j fonts | grep -c "word/fontTable.xml"
  "$BATS_TEST_DIRNAME"/docxbox ls $path_docx --locate -j fonts | grep -c "word/numbering.xml"
  "$BATS_TEST_DIRNAME"/docxbox ls $path_docx --locate -j fonts | grep -c "word/styles.xml"

  run "$BATS_TEST_DIRNAME"/docxbox lslj $path_docx fonts
  [ "     7433  4/11/2020 11:3     word/document.xml" != "${lines[4]}" ]
}

@test "\"docxbox ls filename.docx --locate --json searchString\" lists all files containing given searchString as JSON" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  "$BATS_TEST_DIRNAME"/docxbox ls $path_docx --locate --json fonts | grep -c "word/document.xml"
  "$BATS_TEST_DIRNAME"/docxbox ls $path_docx --locate --json fonts | grep -c "word/fontTable.xml"
  "$BATS_TEST_DIRNAME"/docxbox ls $path_docx --locate --json fonts | grep -c "word/numbering.xml"
  "$BATS_TEST_DIRNAME"/docxbox ls $path_docx --locate --json fonts | grep -c "word/styles.xml"

  run "$BATS_TEST_DIRNAME"/docxbox lslj $path_docx fonts
  [ "     7433  4/11/2020 11:3     word/document.xml" != "${lines[4]}" ]
}
