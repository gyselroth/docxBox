#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

base_command="docxbox lorem filename.docx"

@test "Exit code of \"${base_command}\" is zero" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  run "$BATS_TEST_DIRNAME"/docxbox lorem $path_docx
  [ "$status" -eq 0 ]
}

@test "Output of \"docxbox lorem {missing argument}\" is an error message" {
  run "$BATS_TEST_DIRNAME"/docxbox lorem
  [ "$status" -ne 0 ]
  [ "Missing argument: Filename of DOCX to be extracted" = "${lines[0]}" ]
}

@test "With \"${base_command}\" text gets replaced by dummy text" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  run "$BATS_TEST_DIRNAME"/docxbox lorem $path_docx
  [ "$status" -eq 0 ]

  "$BATS_TEST_DIRNAME"/docxbox txt $path_docx | grep -vc "Culpa ad eiusmod"
}

title="With \"${base_command} newFilename.docx\" "
title+="text gets replaced by dummy text and is saved to new file"
@test "$title" {
  path_docx_1="test/functional/tmp/cp_table_unordered_list_images.docx"
  path_docx_2="test/functional/tmp/lorem.docx"

  "$BATS_TEST_DIRNAME"/docxbox lorem $path_docx_1 $path_docx_2
  ls test/functional/tmp | grep -c lorem.docx
}
