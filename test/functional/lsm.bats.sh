#!/usr/bin/env bats

load _helper

@test "Exit code of \"docxbox lsm filename.docx\" is zero" {
  run $BATS_TEST_DIRNAME/docxbox lsm test/files/docx/table_unordered_list_images.docx
  [ "$status" -eq 0 ]
}

@test "Output of \"docxbox lsm {missing argument}\" is an error message" {
  run "$BATS_TEST_DIRNAME"/docxbox lsm
  [ "$status" -ne 0 ]
  [ "Missing argument: Filename of DOCX to be extracted" = "${lines[0]}" ]
}

@test "Output of \"docxbox lsm filename.docx\" contains information about the xml schema" {
  run $BATS_TEST_DIRNAME/docxbox lsm test/files/docx/table_unordered_list_images.docx
  [ "$status" -eq 0 ]
  [ "xmlSchema: http://schemas.openxmlformats.org/officeDocument/2006" = "${lines[0]}" ]
}

@test "Output of \"docxbox lsm filename.docx\" contains information about the creation time and date" {
  run $BATS_TEST_DIRNAME/docxbox lsm test/files/docx/table_unordered_list_images.docx
  [ "$status" -eq 0 ]
  [ "created: 2017-08-02T11:09:18Z" = "${lines[1]}" ]
}

@test "Output of \"docxbox lsm filename.docx\" contains language information" {
  run $BATS_TEST_DIRNAME/docxbox lsm test/files/docx/table_unordered_list_images.docx
  [ "$status" -eq 0 ]
  [ "language: en-US" = "${lines[2]}" ]
}

@test "Output of \"docxbox lsm filename.docx\" contains information about the revision" {
  run $BATS_TEST_DIRNAME/docxbox lsm test/files/docx/table_unordered_list_images.docx
  [ "$status" -eq 0 ]
  [ "revision: 0" = "${lines[3]}" ]
}
