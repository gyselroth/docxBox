#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

docxbox=""$BATS_TEST_DIRNAME"/docxbox"

unzipped_folder="cp_bio_assay.docx-extracted"

@test "Output of \"docxbox uz {missing argument}\" is an error message" {
  run "${docxbox}" uz
  [ "$status" -ne 0 ]
  [ "Missing argument: Filename of DOCX to be extracted" = "${lines[0]}" ]
}

@test "With of \"docxbox uz filename.docx\" all files are unziped" {
  pattern="^[[:space:]]\{4\}"

  run "${docxbox}" uz test/functional/tmp/cp_bio_assay.docx
  [ "$status" -eq 0 ]

  cat "${unzipped_folder}/word/document.xml" | grep --invert-match "${pattern}"
}

@test "Unziped files are located in project root" {
  ls | grep --count "${unzipped_folder}"

  if [ -d "${unzipped_folder}" ]; then
    rm --recursive "${unzipped_folder}";
  fi
}
