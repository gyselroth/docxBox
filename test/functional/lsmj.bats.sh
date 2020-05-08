#!/usr/bin/env bats

load _helper

@test "Exit code of \"docxbox lsmj filename.docx\" is zero" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  run "$BATS_TEST_DIRNAME"/docxbox lsmj $path_docx
  [ "$status" -eq 0 ]
}

@test "Output of \"docxbox lsmj {missing argument}\" is an error message" {
  run "$BATS_TEST_DIRNAME"/docxbox lsmj
  [ "$status" -ne 0 ]
  [ "Missing argument: Filename of DOCX to be extracted" = "${lines[0]}" ]
}

@test "Output of \"docxbox lsmj filename.docx\" contains information about the xml schema" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  "$BATS_TEST_DIRNAME"/docxbox lsmj $path_docx | grep -c "\"xmlSchema\": \"http://schemas.openxmlformats.org/officeDocument/2006\""
}

@test "Output of \"docxbox lsm filename.docx --json\" contains information about the creation time and date" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  "$BATS_TEST_DIRNAME"/docxbox lsm $path_docx --json | grep -c "\"created\": \"2017-08-02T11:09:18Z\""
}

@test "Output of \"docxbox lsm filename.docx -j\" contains information about the creation time and date" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  "$BATS_TEST_DIRNAME"/docxbox lsm $path_docx -j | grep -c "\"created\": \"2017-08-02T11:09:18Z\""
}

@test "Output of \"docxbox ls filename.docx --meta --json\" contains information about the creation time and date" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  "$BATS_TEST_DIRNAME"/docxbox ls $path_docx --meta --json | grep -c "\"created\": \"2017-08-02T11:09:18Z\""
}

@test "Output of \"docxbox ls filename.docx -mj\" contains information about the creation time and date" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  "$BATS_TEST_DIRNAME"/docxbox ls $path_docx -mj | grep -c "\"created\": \"2017-08-02T11:09:18Z\""
}

@test "Output of \"docxbox lsmj filename.docx\" contains language information" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  "$BATS_TEST_DIRNAME"/docxbox lsmj $path_docx | grep -c "\"language\": \"en-US\""
}

@test "Output of \"docxbox lsmj filename.docx\" contains information about the revision" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  "$BATS_TEST_DIRNAME"/docxbox lsmj $path_docx | grep -c "\"revision\": \"0\""
}
