#!/usr/bin/env bats

load _helper

@test "Output of \"docxbox txt {missing argument}\" is an error message" {
  run "$BATS_TEST_DIRNAME"/docxbox txt
  [ "$status" -ne 0 ]
  [ "Missing argument: Filename of DOCX to be extracted" = "${lines[0]}" ]
}

@test "Output of \"docxbox txt filename.docx\" is the the plain text from given file" {
  "$BATS_TEST_DIRNAME"/docxbox txt test/files/docx/bio_assay.docx | grep -c "NISI IRURE"
  "$BATS_TEST_DIRNAME"/docxbox txt test/files/docx/policy_with_bullets.docx | grep -c "At Officiain Dolore"
  "$BATS_TEST_DIRNAME"/docxbox txt test/functional/tmp/cp_table_unordered_list_images.docx | grep -c "Officia"
}

@test "Output of \"docxbox txt filename.docx -s \" is the segmented plain text from given file" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  "$BATS_TEST_DIRNAME"/docxbox txt $path_docx -s | wc -l | grep -c "105"
  "$BATS_TEST_DIRNAME"/docxbox txt $path_docx | wc -l | grep -c "65"
}

#@test "Output of \"docxbox txt nonExistentFilename.docx\" is an error message" {
#  path_docx="test/files/docx/bio_assay_2.docx"
#
#  $BATS_TEST_DIRNAME/docxbox txt $path_docx | grep -c "Error - File not found:"
#}
