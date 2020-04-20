#!/usr/bin/env bats

load _helper

@test "Output of \"docxbox uz {missing argument}\" is an error message" {
  run "$BATS_TEST_DIRNAME"/docxbox uz
  [ "$status" -ne 0 ]
  [ "Missing argument: Filename of DOCX to be extracted" = "${lines[0]}" ]
}

@test "With of \"docxbox uz filename.docx\" all files are unziped" {
  run "$BATS_TEST_DIRNAME"/docxbox uz test/files/docx/cp_bio_assay.docx
  [ "$status" -eq 0 ]
}

@test "Unziped files are located in project root" {
  ls | grep -c cp_bio_assay.docx-extracted

  if [ -d cp_bio_assay.docx-extracted ]; then
    rm -r cp_bio_assay.docx-extracted;
  fi
}
