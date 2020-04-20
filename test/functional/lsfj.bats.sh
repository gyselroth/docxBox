#!/usr/bin/env bats

load _helper

@test "Exit code of \"docxbox ls filename.docx\" is zero" {
  run $BATS_TEST_DIRNAME/docxbox ls test/files/docx/table_unordered_list_images.docx
  [ "$status" -eq 0 ]
}

@test "Output of \"docxbox lsfj {missing argument}\" is an error message" {
  run "$BATS_TEST_DIRNAME"/docxbox lsfj
  [ "$status" -ne 0 ]
  [ "Missing argument: Filename of DOCX to be extracted" = "${lines[0]}" ]
}

@test "Output of \"docxbox lsfj filename.docx\" contains files' and directories' attributes" {
  "$BATS_TEST_DIRNAME"/docxbox lsfj test/files/docx/table_unordered_list_images.docx | grep "font"
  "$BATS_TEST_DIRNAME"/docxbox lsfj test/files/docx/table_unordered_list_images.docx | grep "altName"
  "$BATS_TEST_DIRNAME"/docxbox lsfj test/files/docx/table_unordered_list_images.docx | grep "charset"
  "$BATS_TEST_DIRNAME"/docxbox lsfj test/files/docx/table_unordered_list_images.docx | grep "family"
  "$BATS_TEST_DIRNAME"/docxbox lsfj test/files/docx/table_unordered_list_images.docx | grep "pitch"
}

@test "Output of \"docxbox lsf filename.docx --json\" contains files' and directories' attributes" {
  "$BATS_TEST_DIRNAME"/docxbox lsf test/files/docx/table_unordered_list_images.docx --json | grep "font"
  "$BATS_TEST_DIRNAME"/docxbox lsf test/files/docx/table_unordered_list_images.docx --json | grep "altName"
  "$BATS_TEST_DIRNAME"/docxbox lsf test/files/docx/table_unordered_list_images.docx --json | grep "charset"
  "$BATS_TEST_DIRNAME"/docxbox lsf test/files/docx/table_unordered_list_images.docx --json | grep "family"
  "$BATS_TEST_DIRNAME"/docxbox lsf test/files/docx/table_unordered_list_images.docx --json | grep "pitch"
}

@test "Output of \"docxbox lsf filename.docx -j\" contains files' and directories' attributes" {
  "$BATS_TEST_DIRNAME"/docxbox lsf test/files/docx/table_unordered_list_images.docx -j | grep "font"
  "$BATS_TEST_DIRNAME"/docxbox lsf test/files/docx/table_unordered_list_images.docx -j | grep "altName"
  "$BATS_TEST_DIRNAME"/docxbox lsf test/files/docx/table_unordered_list_images.docx -j | grep "charset"
  "$BATS_TEST_DIRNAME"/docxbox lsf test/files/docx/table_unordered_list_images.docx -j | grep "family"
  "$BATS_TEST_DIRNAME"/docxbox lsf test/files/docx/table_unordered_list_images.docx -j | grep "pitch"
}

@test "Output of \"docxbox lsfj filename.docx\" contains fontfile-filename" {
  "$BATS_TEST_DIRNAME"/docxbox lsfj test/files/docx/table_unordered_list_images.docx | grep -c "fontTable.xml"
}

@test "Output of \"docxbox lsfj filename.docx\" contains font names" {
  "$BATS_TEST_DIRNAME"/docxbox lsfj test/files/docx/table_unordered_list_images.docx | grep -c "Times New Roman"
  "$BATS_TEST_DIRNAME"/docxbox lsfj test/files/docx/table_unordered_list_images.docx | grep -c "Symbol"
  "$BATS_TEST_DIRNAME"/docxbox lsfj test/files/docx/table_unordered_list_images.docx | grep -c "Arial"
  "$BATS_TEST_DIRNAME"/docxbox lsfj test/files/docx/table_unordered_list_images.docx | grep -c "Liberation Serif"
}

@test "Output of \"docxbox lsfj filename.docx\" can contain alternative font names" {
  "$BATS_TEST_DIRNAME"/docxbox lsfj test/files/docx/table_unordered_list_images.docx | grep -c "Arial Unicode MS"
}

@test "Output of \"docxbox lsfj filename.docx\" contains font-charSets" {
  "$BATS_TEST_DIRNAME"/docxbox lsfj test/files/docx/table_unordered_list_images.docx | grep -c "00"
}

@test "Output of \"docxbox lsfj filename.docx\" contains font-family" {
  "$BATS_TEST_DIRNAME"/docxbox lsfj test/files/docx/table_unordered_list_images.docx | grep -c "roman"
  "$BATS_TEST_DIRNAME"/docxbox lsfj test/files/docx/table_unordered_list_images.docx | grep -c "swiss"
  "$BATS_TEST_DIRNAME"/docxbox lsfj test/files/docx/table_unordered_list_images.docx | grep -c "auto"
}

@test "Output of \"docxbox lsfj filename.docx\" contains font-pitch" {
  "$BATS_TEST_DIRNAME"/docxbox lsfj test/files/docx/table_unordered_list_images.docx | grep -c "variable"
  "$BATS_TEST_DIRNAME"/docxbox lsfj test/files/docx/table_unordered_list_images.docx | grep -c "default"
}
