#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

@test "Output of \"docxbox uzm {missing argument}\" is an error message" {
  run "$BATS_TEST_DIRNAME"/docxbox uzm
  [ "$status" -ne 0 ]
  [ "Missing argument: Filename of DOCX to be extracted" = "${lines[0]}" ]
}

@test "With \"docxbox uzm filename.docx\" only media files are extracted" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  run "$BATS_TEST_DIRNAME"/docxbox uzm $path_docx
}

@test "Unziped files are located in project root" {
  ls | grep -c cp_table_unordered_list_images.docx-media

  if [ -d cp_table_unordered_list_images.docx-media ]; then
    rm -r cp_table_unordered_list_images.docx-media;
  fi
}

@test "With \"docxbox uz filename.docx --media\" only media files are extracted" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  run "$BATS_TEST_DIRNAME"/docxbox uz $path_docx --media
}

@test "Unziped files are located in project root" {
  ls | grep -c cp_table_unordered_list_images.docx-media

  if [ -d cp_table_unordered_list_images.docx-media ]; then
    rm -r cp_table_unordered_list_images.docx-media;
  fi
}

@test "With \"docxbox uz filename.docx -m\" only media files are extracted" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  run "$BATS_TEST_DIRNAME"/docxbox uz $path_docx -m
}

@test "Unziped files are located in project root" {
  ls | grep -c cp_table_unordered_list_images.docx-media

  if [ -d cp_table_unordered_list_images.docx-media-extracted ]; then
    rm -r cp_table_unordered_list_images.docx-media-extracted;
  fi
}
