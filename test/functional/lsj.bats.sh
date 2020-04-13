#!/usr/bin/env bats

load _helper

@test "Output of \"docxbox lsj filename.docx\" contains files' and directories' attributes" {
  "$BATS_TEST_DIRNAME"/docxbox lsj test/files/table_unordered_list_images.docx | grep -c "length"
  "$BATS_TEST_DIRNAME"/docxbox lsj test/files/table_unordered_list_images.docx | grep -c "date"
  "$BATS_TEST_DIRNAME"/docxbox lsj test/files/table_unordered_list_images.docx | grep -c "time"
  "$BATS_TEST_DIRNAME"/docxbox lsj test/files/table_unordered_list_images.docx | grep -c "file"
}

@test "Output of \"docxbox lsj filename.docx\" are contained files" {
  "$BATS_TEST_DIRNAME"/docxbox lsj test/files/table_unordered_list_images.docx | grep -c "[Content_Types].xml"
  "$BATS_TEST_DIRNAME"/docxbox lsj test/files/table_unordered_list_images.docx | grep -c "word/settings.xml"
  "$BATS_TEST_DIRNAME"/docxbox lsj test/files/table_unordered_list_images.docx | grep -c "word/_rels/document.xml.rels"
  "$BATS_TEST_DIRNAME"/docxbox lsj test/files/table_unordered_list_images.docx | grep -c "word/fontTable.xml"
}

@test "Output of \"docxbox lsj filename.docx\" contains files' date and time" {
  "$BATS_TEST_DIRNAME"/docxbox lsj test/files/table_unordered_list_images.docx | grep -c "4/7/2020"
  "$BATS_TEST_DIRNAME"/docxbox lsj test/files/table_unordered_list_images.docx | grep -c "20:14"
}

@test "Output of \"docxbox ls {missing argument}\" is an error message" {
  run "$BATS_TEST_DIRNAME"/docxbox lsj
  [ "$status" -ne 0 ]
  [ "Missing argument: DOCX filename" = "${lines[0]}" ]
}
