#!/usr/bin/env bats

load _helper

@test "Exit code of \"docxbox ls filename.docx\" is zero" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  run "$BATS_TEST_DIRNAME"/docxbox ls $path_docx
  [ "$status" -eq 0 ]
}

@test "Output of \"docxbox ls {missing argument}\" is an error message" {
  run "$BATS_TEST_DIRNAME"/docxbox ls
  [ "$status" -ne 0 ]
  [ "Missing argument: DOCX filename" = "${lines[0]}" ]
}

@test "Output of \"docxbox ls filename.docx\" contains files' and directories' attributes" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  # shellcheck disable=SC2086
  "$BATS_TEST_DIRNAME"/docxbox ls $path_docx | grep -c "Length"
  "$BATS_TEST_DIRNAME"/docxbox ls $path_docx | grep -c "Date"
  "$BATS_TEST_DIRNAME"/docxbox ls $path_docx | grep -c "Time"
  "$BATS_TEST_DIRNAME"/docxbox ls $path_docx | grep -c "Name"
}

@test "Output of \"docxbox ls filename.docx\" is contained files" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  "$BATS_TEST_DIRNAME"/docxbox ls $path_docx | grep -c "[Content_Types].xml"
  "$BATS_TEST_DIRNAME"/docxbox ls $path_docx | grep -c "docProps/app.xml"
  "$BATS_TEST_DIRNAME"/docxbox ls $path_docx | grep -c "docProps/core.xml"
  "$BATS_TEST_DIRNAME"/docxbox ls $path_docx | grep -c "word/_rels/document.xml.rels"
  "$BATS_TEST_DIRNAME"/docxbox ls $path_docx | grep -c "word/charts/chart1.xml"
  "$BATS_TEST_DIRNAME"/docxbox ls $path_docx | grep -c "word/document.xml"
  "$BATS_TEST_DIRNAME"/docxbox ls $path_docx | grep -c "word/fontTable.xml"
  "$BATS_TEST_DIRNAME"/docxbox ls $path_docx | grep -c "word/media/image1.jpeg"
  "$BATS_TEST_DIRNAME"/docxbox ls $path_docx | grep -c "word/numbering.xml"
  "$BATS_TEST_DIRNAME"/docxbox ls $path_docx | grep -c "word/settings.xml"
  "$BATS_TEST_DIRNAME"/docxbox ls $path_docx | grep -c "word/styles.xml"
}

@test "Output of \"docxbox ls filename.docx\" contains amount of contained files" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  "$BATS_TEST_DIRNAME"/docxbox ls $path_docx | grep -c '11 files'
}

@test "Output of \"docxbox ls filename.docx\" contains files' date and time" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  "$BATS_TEST_DIRNAME"/docxbox ls $path_docx | grep -c "4/11/2020"
  "$BATS_TEST_DIRNAME"/docxbox ls $path_docx | grep -c "11:3"
}

@test "Output of \"docxbox ls filename.docx *.file-ending\" contains files with the given file ending" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  "$BATS_TEST_DIRNAME"/docxbox ls $path_docx *.jpeg | grep -c "image1.jpeg"
  "$BATS_TEST_DIRNAME"/docxbox ls $path_docx *.xml | grep -c "9 files"
}
