#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

merge_field="MERGEFIELD"
merge_format="\* MERGEFORMAT"
path_docx="test/functional/tmp/cp_mergefields.docx"

@test "Exit code of \"docxbox lsd filename.docx\" is zero" {
  run "$BATS_TEST_DIRNAME"/docxbox lsd $path_docx
  [ "$status" -eq 0 ]
}

@test "Output of \"docxbox lsd {missing argument}\" is an error message" {
  run "$BATS_TEST_DIRNAME"/docxbox lsd
  [ "$status" -ne 0 ]
  [ "Missing argument: Filename of DOCX to be extracted" = "${lines[0]}" ]
}

@test "With \"docxbox lsd filename.docx\" the fields in the docx are listed" {
  "$BATS_TEST_DIRNAME/"docxbox lsd $path_docx | grep -c "$merge_field"
  "$BATS_TEST_DIRNAME/"docxbox lsd $path_docx | grep -c "$merge_format"
}

title="With \"docxbox ls filename.docx --fields\" "
title+="the fields in the docx are listed"
@test "$title" {
  "$BATS_TEST_DIRNAME/"docxbox lsd $path_docx | grep -c "$merge_field"
  "$BATS_TEST_DIRNAME/"docxbox lsd $path_docx | grep -c "$merge_format"
}

@test "With \"docxbox ls filename.docx -d\" the fields in the docx are listed" {
  "$BATS_TEST_DIRNAME/"docxbox ls $path_docx -d | grep -c "$merge_field"
  "$BATS_TEST_DIRNAME/"docxbox ls $path_docx -d | grep -c "$merge_format"
}
