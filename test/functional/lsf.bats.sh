#!/usr/bin/env bats

load _helper

@test "Exit code of \"docxbox ls filename.docx\" is zero" {
  run $BATS_TEST_DIRNAME/docxbox ls test/files/docx/table_unordered_list_images.docx
  [ "$status" -eq 0 ]
}

@test "Output of \"docxbox lsf {missing argument}\" is an error message" {
  run $BATS_TEST_DIRNAME/docxbox lsf
  [ "$status" -ne 0 ]
  [ "Missing argument: Filename of DOCX to be extracted" = "${lines[0]}" ]
}

@test "Output of \"docxbox lsf filename.docx\" contains ground informations" {
  run $BATS_TEST_DIRNAME/docxbox lsf test/files/docx/table_unordered_list_images.docx
  [ "$status" -eq 0 ]
  [ "word/fontTable.xml lists 12 fonts:" = "${lines[0]}" ]
}

@test "Output of \"docxbox lsf filename.docx\" contains files' and directories' attributes" {
  "$BATS_TEST_DIRNAME"/docxbox lsf test/files/docx/table_unordered_list_images.docx | grep "Font"
  "$BATS_TEST_DIRNAME"/docxbox lsf test/files/docx/table_unordered_list_images.docx | grep "AltName"
  "$BATS_TEST_DIRNAME"/docxbox lsf test/files/docx/table_unordered_list_images.docx | grep "CharSet"
  "$BATS_TEST_DIRNAME"/docxbox lsf test/files/docx/table_unordered_list_images.docx | grep "Family"
  "$BATS_TEST_DIRNAME"/docxbox lsf test/files/docx/table_unordered_list_images.docx | grep "Pitch"
}

@test "Output of \"docxbox lsf filename.docx\" contains fontfile-filename" {
  "$BATS_TEST_DIRNAME"/docxbox lsf test/files/docx/table_unordered_list_images.docx | grep -c "fontTable.xml"
}

@test "Output of \"docxbox lsf filename.docx\" contains amount fonts" {
  "$BATS_TEST_DIRNAME"/docxbox lsf test/files/table_unordered_list_images.docx | grep -c "12 fonts"
}

@test "Output of \"docxbox lsf filename.docx\" contains font names" {
  "$BATS_TEST_DIRNAME"/docxbox lsf test/files/docx/table_unordered_list_images.docx | grep -c "Times New Roman"
  "$BATS_TEST_DIRNAME"/docxbox lsf test/files/docx/table_unordered_list_images.docx | grep -c "Symbol"
  "$BATS_TEST_DIRNAME"/docxbox lsf test/files/docx/table_unordered_list_images.docx | grep -c "Arial"
  "$BATS_TEST_DIRNAME"/docxbox lsf test/files/docx/table_unordered_list_images.docx | grep -c "Liberation Serif"
}

@test "Output of \"docxbox lsf filename.docx\" can contain alternative font names" {
  "$BATS_TEST_DIRNAME"/docxbox lsf test/files/docx/table_unordered_list_images.docx | grep -c "Arial Unicode MS"
}

@test "Output of \"docxbox lsf filename.docx\" contains font-charSets" {
  "$BATS_TEST_DIRNAME"/docxbox lsf test/files/docx/table_unordered_list_images.docx | grep -c "00"
}

@test "Output of \"docxbox lsf filename.docx\" contains font-family" {
  "$BATS_TEST_DIRNAME"/docxbox lsf test/files/docx/table_unordered_list_images.docx | grep -c "roman"
  "$BATS_TEST_DIRNAME"/docxbox lsf test/files/docx/table_unordered_list_images.docx | grep -c "swiss"
  "$BATS_TEST_DIRNAME"/docxbox lsf test/files/docx/table_unordered_list_images.docx | grep -c "auto"
}

@test "Output of \"docxbox lsf filename.docx\" contains font-pitch" {
  "$BATS_TEST_DIRNAME"/docxbox lsf test/files/docx/table_unordered_list_images.docx | grep -c "variable"
  "$BATS_TEST_DIRNAME"/docxbox lsf test/files/docx/table_unordered_list_images.docx | grep -c "default"
}
