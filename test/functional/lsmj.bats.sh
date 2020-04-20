#!/usr/bin/env bats

load _helper

@test "Exit code of \"docxbox lsmj filename.docx\" is zero" {
  run $BATS_TEST_DIRNAME/docxbox lsmj test/files/docx/table_unordered_list_images.docx
  [ "$status" -eq 0 ]
}

@test "Output of \"docxbox lsmj {missing argument}\" is an error message" {
  run "$BATS_TEST_DIRNAME"/docxbox lsmj
  [ "$status" -ne 0 ]
  [ "Missing argument: Filename of DOCX to be extracted" = "${lines[0]}" ]
}

@test "Output of \"docxbox lsmj filename.docx\" contains information about the xml schema" {
  $BATS_TEST_DIRNAME/docxbox lsmj test/files/docx/table_unordered_list_images.docx | grep -c "\"xmlSchema\": \"http://schemas.openxmlformats.org/officeDocument/2006\""
}

@test "Output of \"docxbox lsm filename.docx --json\" contains information about the creation time and date" {
  $BATS_TEST_DIRNAME/docxbox lsm test/files/docx/table_unordered_list_images.docx --json | grep -c "\"created\": \"2017-08-02T11:09:18Z\""
}

@test "Output of \"docxbox lsm filename.docx -j\" contains information about the creation time and date" {
  $BATS_TEST_DIRNAME/docxbox lsm test/files/docx/table_unordered_list_images.docx -j | grep -c "\"created\": \"2017-08-02T11:09:18Z\""
}

@test "Output of \"docxbox lsmj filename.docx\" contains language information" {
  $BATS_TEST_DIRNAME/docxbox lsmj test/files/docx/table_unordered_list_images.docx | grep -c "\"language\": \"en-US\""
}

@test "Output of \"docxbox lsmj filename.docx\" contains information about the revision" {
  $BATS_TEST_DIRNAME/docxbox lsmj test/files/docx/table_unordered_list_images.docx | grep -c "\"revision\": \"0\""
}
