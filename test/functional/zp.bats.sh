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
  if [ ! -d test/files/docx/unziped ]; then
    mkdir test/files/docx/unziped;
    path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"
    unzip $path_docx -d test/files/docx/unziped;
  fi

  path_docx="test/files/docx/zp_table_unordered_list_images.docx"

  run "$BATS_TEST_DIRNAME"/docxbox zp test/files/docx/unziped $path_docx
  [ "$status" -eq 0 ]
}

@test "Zipped file is located in project root" {
  ls test/files/docx | grep -c zp_table_unordered_list_images.docx

# cleanup
  if [ -d test/files/docx/unziped ]; then
    rm -r test/files/docx/unziped;
  fi

  if [ -f test/files/docx/zp_table_unordered_list_images.docx ]; then
    rm test/files/docx/zp_table_unordered_list_images.docx;
  fi
}
