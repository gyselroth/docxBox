#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

@test "Output of \"docxbox zp {missing argument}\" is an error message" {
  run "$BATS_TEST_DIRNAME"/docxbox zp
  [ "$status" -ne 0 ]
  [ "Missing argument: Path of directory to be zipped" = "${lines[0]}" ]
}

title="With \"docxbox zp directory /path-to-file/filename.docx\" "
title+="a directory can be zipped into a docx"
@test "$title" {
  if [ ! -d test/functional/tmp/unziped ]; then
    mkdir test/functional/tmp/unziped;
    path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"
    unzip $path_docx -d test/functional/tmp/unziped;
  fi

  path_new_docx="test/functional/tmp/zp_table_unordered_list_images.docx"

  run "$BATS_TEST_DIRNAME"/docxbox zp test/functional/tmp/unziped $path_new_docx
  [ "$status" -eq 0 ]

  ls test/functional/tmp | grep -c zp_table_unordered_list_images.docx
}
