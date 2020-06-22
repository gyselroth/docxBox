#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

docxbox="$BATS_TEST_DIRNAME/docxbox"

path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"
unzipped_docx_directory="cp_table_unordered_list_images.docx-extracted"

@test "Output of \"docxbox zpc {missing argument}\" is an error message" {
  pattern="docxBox Error - Missing argument: Path of directory to be zipped"

  run "${docxbox}" zpc
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]
}

@test "Output of \"docxbox zpc directory {missing argument}\" is an error message" {
  pattern="docxBox Error - Missing argument: Filename of docx to be created"

  run "${docxbox}" zpc "${unzipped_docx_directory}"
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]
}

title="With \"docxbox zp directory /path-to-file/filename.docx\" "
title+="a directory can be zipped into a docx"
@test "$title" {
  if [ ! -d "${unzipped_docx_directory}" ]; then
    "${docxbox}" uzi "${path_docx}"
  fi

  path_new_docx="test/functional/tmp/zp_table_unordered_list_images.docx"

  run "${docxbox}" zpc "${unzipped_docx_directory}" "${path_new_docx}"
  [ "$status" -eq 0 ]

  ls test/functional/tmp | grep -c zp_table_unordered_list_images.docx

  if [ -d "${unzipped_docx_directory}" ]; then
    rm --recursive "${unzipped_docx_directory}"
  fi
}
