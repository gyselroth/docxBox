#!/usr/bin/env bats

load _helper

@test "Exit code of \"docxbox lorem filename.docx\" is zero" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  run "$BATS_TEST_DIRNAME"/docxbox lorem $path_docx
  [ "$status" -eq 0 ]
}

@test "Output of \"docxbox lorem {missing argument}\" is an error message" {
  run "$BATS_TEST_DIRNAME"/docxbox lorem
  [ "$status" -ne 0 ]
  [ "Missing argument: Filename of DOCX to be extracted" = "${lines[0]}" ]
}

@test "With \"docxbox lorem filename.docx\" text gets replaced by dummy text" {
  path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

  "$BATS_TEST_DIRNAME"/docxbox txt $path_docx | grep -vc "Culpa ad eiusmod"
}

@test "With \"docxbox lorem filename.docx newFilename.docx\" text gets replaced by dummy text and is saved to new file" {
  path_docx_1="test/functional/tmp/cp_table_unordered_list_images.docx"
  path_docx_2="test/functional/tmp/lorem.docx"

  "$BATS_TEST_DIRNAME"/docxbox lorem $path_docx_1 $path_docx_2
  ls test/functional/tmp | grep -c lorem.docx
}
