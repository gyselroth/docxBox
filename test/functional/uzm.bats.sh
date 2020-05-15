#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

description="only media files are extracted"
unzipped_docx="cp_table_unordered_list_images.docx-media-extracted"

@test "Output of \"docxbox uzm {missing argument}\" is an error message" {
  run "$BATS_TEST_DIRNAME"/docxbox uzm
  [ "$status" -ne 0 ]
  [ "Missing argument: Filename of DOCX to be extracted" = "${lines[0]}" ]
}

@test "With \"docxbox uzm filename.docx\" ${description}" {
  run "$BATS_TEST_DIRNAME"/docxbox uzm "${path_docx}"
}

@test "Unzipped files are located in project root" {
  ls | grep -c "${unzipped_docx}"

  if [ -d "${unzipped_docx}" ]; then
    rm -r "${unzipped_docx}";
  fi
}

@test "With \"docxbox uz filename.docx --media\" ${description}" {
  run "$BATS_TEST_DIRNAME"/docxbox uz "${path_docx}" --media
}

@test "Unzipped files are located in project root after running uz --media " {
  ls | grep -c "${unzipped_docx}"

  if [ -d "${unzipped_docx}" ]; then
    rm -r "${unzipped_docx}";
  fi
}

@test "With \"docxbox uz filename.docx -m\" ${description}" {
  run "$BATS_TEST_DIRNAME"/docxbox uz "${path_docx}" -m
}

@test "Unzipped files are located in project root after running uz -m" {
  ls | grep -c "${unzipped_docx}"

  if [ -d "${unzipped_docx}" ]; then
    rm -r "${unzipped_docx}";
  fi
}
