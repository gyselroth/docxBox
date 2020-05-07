#!/usr/bin/env bats

load _helper

@test "Output of \"docxbox sfv {missing argument}\" is an error message" {
  run "$BATS_TEST_DIRNAME"/docxbox sfv
  [ "$status" -ne 0 ]
  [ "Missing argument: Filename of DOCX to be extracted" = "${lines[0]}" ]
}

@test "Output of \"docxbox sfv filename.docx {missing argument}\" is an error message" {
  run "$BATS_TEST_DIRNAME"/docxbox sfv test/functional/tmp/cp_bio_assay.docx
  [ "$status" -ne 0 ]
  [ "Missing argument: Field identifier" = "${lines[0]}" ]
}

@test "Output of \"docxbox sfv filename.docx fieldIdentifier {missing argument} is an error message" {
  run "$BATS_TEST_DIRNAME"/docxbox sfv test/functional/tmp/cp_bio_assay.docx "MERGEFILED Schueler_Anrede"
  [ "$status" -ne 0 ]
  [ "Missing argument: Value to be set" = "${lines[0]}" ]
}

@test "Output of \"docxbox sfv filename.docx \"fieldIdentifier\" fieldValue is an error message" {
  run "$BATS_TEST_DIRNAME"/docxbox sfv test/functional/tmp/cp_bio_assay.docx "MERGEFILED Schueler_Anrede" someName
  "$BATS_TEST_DIRNAME"/docxbox lsd test/functional/tmp/cp_bio_assay.docx | grep -c "someName"
}
