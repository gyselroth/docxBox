#!/usr/bin/env bats

load _helper

@test "Exit code of \"docxbox lsij filename.docx\" is zero" {
  run "$BATS_TEST_DIRNAME"/docxbox lsij test/files/docx/table_unordered_list_images.docx
  [ "$status" -eq 0 ]
}

@test "Output of \"docxbox lsij {missing argument}\" is an error message" {
  run "$BATS_TEST_DIRNAME"/docxbox lsij
  [ "$status" -ne 0 ]
  [ "Missing argument: Filename of DOCX to be extracted" = "${lines[0]}" ]
}

@test "Output of \"docxbox lsij filename.docx\" is contained images as JSON" {
  "$BATS_TEST_DIRNAME"/docxbox lsij test/files/docx/table_unordered_list_images.docx | grep -c "image1.jpeg"
}

@test "Output of \"docxbox lsi filename.docx --json\" are contained images as JSON" {
  "$BATS_TEST_DIRNAME"/docxbox lsi test/files/docx/table_unordered_list_images.docx --json | grep -c "image1.jpeg"
}

@test "Output of \"docxbox lsi filename.docx -j\" are contained images as JSON" {
  $BATS_TEST_DIRNAME/docxbox lsi test/files/docx/table_unordered_list_images.docx -j | grep -c "image1.jpeg"
}
