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
  run "$BATS_TEST_DIRNAME"/docxbox sfv test/functional/tmp/cp_bio_assay.docx "MERGEFIELD  Schueler_Anrede"
  [ "$status" -ne 0 ]
  [ "Missing argument: Value to be set" = "${lines[0]}" ]
}

@test "Output of \"docxbox sfv filename.docx \"fieldIdentifier\" fieldValue changes the value of the given field" {
  path_docx="test/functional/tmp/cp_file_with_mergefields.docx"

  run "$BATS_TEST_DIRNAME"/docxbox sfv $path_docx "MERGEFIELD  Schueler_Anrede" foobar
  [ "$status" -eq 0 ]

  "$BATS_TEST_DIRNAME"/docxbox txt $path_docx | grep -c "foobar"
}
