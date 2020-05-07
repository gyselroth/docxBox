#!/usr/bin/env bats

load _helper

@test "Exit code of \"docxbox lsi filename.docx\" is zero" {
  run "$BATS_TEST_DIRNAME"/docxbox lsi test/functional/tmp/cp_table_unordered_list_images.docx
  [ "$status" -eq 0 ]
}

@test "Output of \"docxbox lsi {missing argument}\" is an error message" {
  run "$BATS_TEST_DIRNAME"/docxbox lsi
  [ "$status" -ne 0 ]
  [ "Missing argument: Filename of DOCX to be extracted" = "${lines[0]}" ]
}

@test "Output of \"docxbox lsi filename.docx\" is contained images" {
  run "$BATS_TEST_DIRNAME"/docxbox lsi test/functional/tmp/cp_table_unordered_list_images.docx
  [ "$status" -eq 0 ]
  "$BATS_TEST_DIRNAME"/docxbox lsi test/functional/tmp/cp_table_unordered_list_images.docx | grep -c image1.jpeg
}
