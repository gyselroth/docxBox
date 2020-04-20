#!/usr/bin/env bats

load _helper

@test "Exit code of \"docxbox ls filename.docx\" is zero" {
  run "$BATS_TEST_DIRNAME"/docxbox ls test/files/table_unordered_list_images.docx
  [ "$status" -eq 0 ]
}

@test "Output of \"docxbox ls {missing argument}\" is an error message" {
  run "$BATS_TEST_DIRNAME"/docxbox ls
  [ "$status" -ne 0 ]
  [ "Missing argument: DOCX filename" = "${lines[0]}" ]
}

@test "Output of \"docxbox ls filename.docx\" contains files' and directories' attributes" {
  # shellcheck disable=SC2086
  "$BATS_TEST_DIRNAME"/docxbox ls test/files/docx/table_unordered_list_images.docx | grep -c "Length"
  "$BATS_TEST_DIRNAME"/docxbox ls test/files/docx/table_unordered_list_images.docx | grep -c "Date"
  "$BATS_TEST_DIRNAME"/docxbox ls test/files/docx/table_unordered_list_images.docx | grep -c "Time"
  "$BATS_TEST_DIRNAME"/docxbox ls test/files/docx/table_unordered_list_images.docx | grep -c "Name"
}

@test "Output of \"docxbox ls filename.docx\" is contained files" {
  "$BATS_TEST_DIRNAME"/docxbox ls test/docx/files/table_unordered_list_images.docx | grep -c "[Content_Types].xml"
  "$BATS_TEST_DIRNAME"/docxbox ls test/docx/files/table_unordered_list_images.docx | grep -c "docProps/app.xml"
  "$BATS_TEST_DIRNAME"/docxbox ls test/docx/files/table_unordered_list_images.docx | grep -c "docProps/core.xml"
  "$BATS_TEST_DIRNAME"/docxbox ls test/docx/files/table_unordered_list_images.docx | grep -c "word/_rels/document.xml.rels"
  "$BATS_TEST_DIRNAME"/docxbox ls test/docx/files/table_unordered_list_images.docx | grep -c "word/charts/chart1.xml"
  "$BATS_TEST_DIRNAME"/docxbox ls test/docx/files/table_unordered_list_images.docx | grep -c "word/document.xml"
  "$BATS_TEST_DIRNAME"/docxbox ls test/docx/files/table_unordered_list_images.docx | grep -c "word/fontTable.xml"
  "$BATS_TEST_DIRNAME"/docxbox ls test/docx/files/table_unordered_list_images.docx | grep -c "word/media/image1.jpeg"
  "$BATS_TEST_DIRNAME"/docxbox ls test/docx/files/table_unordered_list_images.docx | grep -c "word/numbering.xml"
  "$BATS_TEST_DIRNAME"/docxbox ls test/docx/files/table_unordered_list_images.docx | grep -c "word/settings.xml"
  "$BATS_TEST_DIRNAME"/docxbox ls test/docx/files/table_unordered_list_images.docx | grep -c "word/styles.xml"
}

@test "Output of \"docxbox ls filename.docx\" contains amount of contained files" {
  "$BATS_TEST_DIRNAME"/docxbox ls test/files/docx/table_unordered_list_images.docx | grep -c '11 files'
}

@test "Output of \"docxbox ls filename.docx\" contains files' date and time" {
  "$BATS_TEST_DIRNAME"/docxbox ls test/files/docx/table_unordered_list_images.docx | grep -c "04/11/2020"
  "$BATS_TEST_DIRNAME"/docxbox ls test/files/docx/table_unordered_list_images.docx | grep -c "11:03"
}
