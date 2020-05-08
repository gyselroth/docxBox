#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

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

title="With \"docxbox lsdj filename.docx\" "
title+="the fields in the docx are listed as JSON"
@test "$title" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  pattern="table_unordered_list_images.docx-"
  "$BATS_TEST_DIRNAME/"docxbox lsdj $path_docx | grep -c $pattern

  pattern="/word/document.xml"
  "$BATS_TEST_DIRNAME/"docxbox lsdj $path_docx | grep -c $pattern
}

title="With \"docxbox ls filename.docx --fields --json\" "
title+="the fields in the docx are listed as JSON"
@test "$title" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  pattern="table_unordered_list_images.docx-"
  "$BATS_TEST_DIRNAME/"docxbox ls $path_docx --fields --json | grep -c $pattern

  pattern="/word/document.xml"
  "$BATS_TEST_DIRNAME/"docxbox ls $path_docx --fields --json | grep -c $pattern
}

title="With \"docxbox ls filename.docx -dj\" "
title+="the fields in the docx are listed as JSON"
@test "$title" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  pattern="table_unordered_list_images.docx-"
  "$BATS_TEST_DIRNAME/"docxbox ls $path_docx -dj | grep -c $pattern

  "$BATS_TEST_DIRNAME/"docxbox ls $path_docx -dj | grep -c "/word/document.xml"
}

title="With \"docxbox lsd filename.docx --json\" "
title+="the fields in the docx are listed as JSON"
@test "$title" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  pattern="table_unordered_list_images.docx-"
  "$BATS_TEST_DIRNAME/"docxbox lsd $path_docx --json | grep -c $pattern

  pattern="/word/document.xml"
  "$BATS_TEST_DIRNAME/"docxbox lsd $path_docx --json | grep -c $pattern
}
