#!/usr/bin/env bats

load _helper

@test "Exit code of \"docxbox ls filename.docx\" is zero" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  run "$BATS_TEST_DIRNAME"/docxbox ls $path_docx
  [ "$status" -eq 0 ]
}

@test "Output of \"docxbox lsfj {missing argument}\" is an error message" {
  run "$BATS_TEST_DIRNAME"/docxbox lsfj
  [ "$status" -ne 0 ]
  [ "Missing argument: Filename of DOCX to be extracted" = "${lines[0]}" ]
}

@test "Output of \"docxbox lsfj filename.docx\" contains files' and directories' attributes" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  "$BATS_TEST_DIRNAME"/docxbox lsfj $path_docx | grep "font"
  "$BATS_TEST_DIRNAME"/docxbox lsfj $path_docx | grep "altName"
  "$BATS_TEST_DIRNAME"/docxbox lsfj $path_docx | grep "charset"
  "$BATS_TEST_DIRNAME"/docxbox lsfj $path_docx | grep "family"
  "$BATS_TEST_DIRNAME"/docxbox lsfj $path_docx | grep "pitch"
  "$BATS_TEST_DIRNAME"/docxbox lsfj $path_docx | grep "font"
  "$BATS_TEST_DIRNAME"/docxbox lsfj $path_docx | grep "altName"
  "$BATS_TEST_DIRNAME"/docxbox lsfj $path_docx | grep "charset"
  "$BATS_TEST_DIRNAME"/docxbox lsfj $path_docx | grep "family"
  "$BATS_TEST_DIRNAME"/docxbox lsfj $path_docx | grep "pitch"
}

@test "Output of \"docxbox lsf filename.docx --json\" contains files' and directories' attributes" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  "$BATS_TEST_DIRNAME"/docxbox lsf $path_docx --json | grep "font"
  "$BATS_TEST_DIRNAME"/docxbox lsf $path_docx --json | grep "altName"
  "$BATS_TEST_DIRNAME"/docxbox lsf $path_docx --json | grep "charset"
  "$BATS_TEST_DIRNAME"/docxbox lsf $path_docx --json | grep "family"
  "$BATS_TEST_DIRNAME"/docxbox lsf $path_docx --json | grep "pitch"
}

@test "Output of \"docxbox lsf filename.docx -j\" contains files' and directories' attributes" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  "$BATS_TEST_DIRNAME"/docxbox lsf $path_docx -j | grep "font"
  "$BATS_TEST_DIRNAME"/docxbox lsf $path_docx -j | grep "altName"
  "$BATS_TEST_DIRNAME"/docxbox lsf $path_docx -j | grep "charset"
  "$BATS_TEST_DIRNAME"/docxbox lsf $path_docx -j | grep "family"
  "$BATS_TEST_DIRNAME"/docxbox lsf $path_docx -j | grep "pitch"
}

@test "Output of \"docxbox ls filename.docx --fonts --json\" contains files' and directories' attributes" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  "$BATS_TEST_DIRNAME"/docxbox ls $path_docx --fonts --json | grep "altName"
  "$BATS_TEST_DIRNAME"/docxbox ls $path_docx --fonts --json | grep "font"
  "$BATS_TEST_DIRNAME"/docxbox ls $path_docx --fonts --json | grep "charset"
  "$BATS_TEST_DIRNAME"/docxbox ls $path_docx --fonts --json | grep "family"
  "$BATS_TEST_DIRNAME"/docxbox ls $path_docx --fonts --json | grep "pitch"
}

@test "Output of \"docxbox ls filename.docx -fj\" contains files' and directories' attributes" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  "$BATS_TEST_DIRNAME"/docxbox ls $path_docx -fj | grep "altName"
  "$BATS_TEST_DIRNAME"/docxbox ls $path_docx -fj | grep "font"
  "$BATS_TEST_DIRNAME"/docxbox ls $path_docx -fj | grep "charset"
  "$BATS_TEST_DIRNAME"/docxbox ls $path_docx -fj | grep "family"
  "$BATS_TEST_DIRNAME"/docxbox ls $path_docx -fj | grep "pitch"
}

@test "Output of \"docxbox lsfj filename.docx\" contains fontfile-filename" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  "$BATS_TEST_DIRNAME"/docxbox lsfj $path_docx | grep -c "fontTable.xml"
}

@test "Output of \"docxbox lsfj filename.docx\" contains font names" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  "$BATS_TEST_DIRNAME"/docxbox lsfj $path_docx | grep -c "Times New Roman"
  "$BATS_TEST_DIRNAME"/docxbox lsfj $path_docx | grep -c "Symbol"
  "$BATS_TEST_DIRNAME"/docxbox lsfj $path_docx | grep -c "Arial"
  "$BATS_TEST_DIRNAME"/docxbox lsfj $path_docx | grep -c "Liberation Serif"
}

@test "Output of \"docxbox lsfj filename.docx\" can contain alternative font names" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  "$BATS_TEST_DIRNAME"/docxbox lsfj $path_docx | grep -c "Arial Unicode MS"
}

@test "Output of \"docxbox lsfj filename.docx\" contains font-charSets" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  "$BATS_TEST_DIRNAME"/docxbox lsfj $path_docx | grep -c "00"
}

@test "Output of \"docxbox lsfj filename.docx\" contains font-family" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  "$BATS_TEST_DIRNAME"/docxbox lsfj $path_docx | grep -c "roman"
  "$BATS_TEST_DIRNAME"/docxbox lsfj $path_docx | grep -c "swiss"
  "$BATS_TEST_DIRNAME"/docxbox lsfj $path_docx | grep -c "auto"
}

@test "Output of \"docxbox lsfj filename.docx\" contains font-pitch" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  "$BATS_TEST_DIRNAME"/docxbox lsfj $path_docx | grep -c "variable"
  "$BATS_TEST_DIRNAME"/docxbox lsfj $path_docx | grep -c "default"
}
