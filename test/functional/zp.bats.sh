#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

unzipped_docx="test/functional/tmp/unzipped"

@test "Output of \"docxbox zp {missing argument}\" is an error message" {
  run "$BATS_TEST_DIRNAME"/docxbox zp
  [ "$status" -ne 0 ]
  [ "Missing argument: Path of directory to be zipped" = "${lines[0]}" ]
}

title="With \"docxbox zp directory /path-to-file/filename.docx\" "
title+="a directory can be zipped into a docx"
@test "$title" {
  if [ ! -d "${unzipped_docx}" ]; then
    mkdir "${unzipped_docx}";
    path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"
    unzip $path_docx -d "${unzipped_docx}";
  fi

  path_new_docx="test/functional/tmp/zp_table_unordered_list_images.docx"

  run "$BATS_TEST_DIRNAME"/docxbox zp "${unzipped_docx}" "${path_new_docx}"
  [ "$status" -eq 0 ]

  ls test/functional/tmp | grep -c zp_table_unordered_list_images.docx
}
