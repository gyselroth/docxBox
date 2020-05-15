#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

docxbox=""$BATS_TEST_DIRNAME"/docxbox"
path_docx="test/functional/tmp/cp_bio_assay.docx"

unzipped_folder="cp_bio_assay.docx-extracted"

@test "Output of \"docxbox uzi {missing argument}\" is an error message" {
  run "${docxbox}" uzi
  [ "$status" -ne 0 ]
  [ "Missing argument: Filename of DOCX to be extracted" = "${lines[0]}" ]
}

@test "With of \"docxbox uzi filename.docx\" all files are unzipped" {
  run "${docxbox}" uzi "${path_docx}"
  [ "$status" -eq 0 ]
}

@test "Unziped files are located in project root" {
  ls | grep --count "${unzipped_folder}"

  if [ -d "${unzipped_folder}" ]; then
    rm --recursive "${unzipped_folder}";
  fi
}

@test "With of \"docxbox uz filename.docx -i\" all files are unzipped" {
  run "${docxbox}" uz "${path_docx}" -i
  [ "$status" -eq 0 ]
}

@test "Unziped files are located in project root after running uz -i" {
  ls | grep --count "${unzipped_folder}"

  if [ -d "${unzipped_folder}" ]; then
    rm --recursive "${unzipped_folder}";
  fi
}

@test "With of \"docxbox uz filename.docx --indent\" all files are unzipped" {
  run "${docxbox}" uz "${path_docx}" --indent
  [ "$status" -eq 0 ]
}

@test "Unziped files are located in project root after running uz --indent" {
  ls | grep --count "${unzipped_folder}"

  if [ -d "${unzipped_folder}" ]; then
    rm --recursive "${unzipped_folder}";
  fi
}
