#!/usr/bin/env bats

load _helper

@test "Output of \"docxbox rpt {missing filename}\" is an error message" {
  run "$BATS_TEST_DIRNAME"/docxbox rpt
  [ "$status" -ne 0 ]
  [ "Missing argument: DOCX filename" = "${lines[0]}" ]
}

@test "Output of \"docxbox rpt filename.docx {missing arguments}\" is an error message" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  run "$BATS_TEST_DIRNAME"/docxbox rpt $path_docx
  [ "$status" -ne 0 ]
  [ "Missing argument: String to be found (and replaced)" = "${lines[0]}" ]
}

@test "Output of \"docxbox rpt filename.docx stringToBeReplaced {missing argument}\" is an error message" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  run "$BATS_TEST_DIRNAME"/docxbox rpt $path_docx toBeReplaced
  [ "$status" -ne 0 ]
  [ "Missing argument: Replacement" = "${lines[0]}" ]
}

@test "With \"docxbox rpt filename.docx stringToBeReplaced replacementString\" the stringToBeReplaced gets replaced" {
  path_docx="test/functional/tmp/cp_bio_assay.docx"

  run "$BATS_TEST_DIRNAME"/docxbox rpt $path_docx Lorem Dorem
  [ "$status" -eq 0 ]
  "$BATS_TEST_DIRNAME"/docxbox txt $path_docx | grep -c Dorem
}

@test "With \"docxbox rpt filename.docx stringToBeReplaced replacementString newFile.docx\" the stringToBeReplaced gets replaced and is saved to new file" {
  path_docx_in="test/functional/tmp/cp_bio_assay.docx"
  path_docx_out="test/functional/tmp/replacedString.docx"

  run "$BATS_TEST_DIRNAME"/docxbox rpt $path_docx_in Lorem Dorem $path_docx_out
  [ "$status" -eq 0 ]
  "$BATS_TEST_DIRNAME"/docxbox txt test/functional/tmp/replacedString.docx | grep -c Dorem
}
