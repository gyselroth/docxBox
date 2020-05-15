#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

docxbox=""$BATS_TEST_DIRNAME"/docxbox"
path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

description="only media files are extracted"
unzipped_docx="cp_table_unordered_list_images.docx-media-extracted"

@test "Output of \"docxbox uzm {missing argument}\" is an error message" {
  run "${docxbox}" uzm
  [ "$status" -ne 0 ]
  [ "Missing argument: Filename of DOCX to be extracted" = "${lines[0]}" ]
}

@test "With \"docxbox uzm filename.docx\" ${description}" {
  run "${docxbox}" uzm "${path_docx}"
}

@test "Unzipped files are located in project root" {
  ls | grep --count "${unzipped_docx}"

  if [ -d "${unzipped_docx}" ]; then
    rm --recursive "${unzipped_docx}";
  fi
}

@test "With \"docxbox uz filename.docx --media\" ${description}" {
  run "${docxbox}" uz "${path_docx}" --media
}

@test "Unzipped files are located in project root after running uz --media " {
  ls | grep --count "${unzipped_docx}"

  if [ -d "${unzipped_docx}" ]; then
    rm --recursive "${unzipped_docx}";
  fi
}

@test "With \"docxbox uz filename.docx -m\" ${description}" {
  run "${docxbox}" uz "${path_docx}" -m
}

@test "Unzipped files are located in project root after running uz -m" {
  ls | grep --count "${unzipped_docx}"

  if [ -d "${unzipped_docx}" ]; then
    rm --recursive "${unzipped_docx}";
  fi
}
