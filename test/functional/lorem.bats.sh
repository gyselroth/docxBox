#!/usr/bin/env bats

load _helper

@test "Exit code of \"docxbox lorem filename.docx\" is zero" {
  run "$BATS_TEST_DIRNAME"/docxbox lorem test/files/docx/cp_table_unordered_list_images.docx
  [ "$status" -eq 0 ]
}

@test "Output of \"docxbox lsd {missing argument}\" is an error message" {
  run "$BATS_TEST_DIRNAME"/docxbox lorem
  [ "$status" -ne 0 ]
  [ "Missing argument: Filename of DOCX to be extracted" = "${lines[0]}" ]
}

@test "With \"docxbox lorem filename.docx\" text gets replaced by dummy text" {
  "$BATS_TEST_DIRNAME"/docxbox txt test/files/docx/cp_table_unordered_list_images.docx | grep -vc "Culpa ad eiusmod"
}

@test "With \"docxbox lorem filename.docx newFilename.docx\" text gets replaced by dummy text and is saved to new file" {
  "$BATS_TEST_DIRNAME"/docxbox lorem test/files/docx/cp_table_unordered_list_images.docx test/files/docx/lorem.docx
  ls test/files/docx | grep -c lorem.docx

  if [ -f test/files/docx/lorem.docx ]; then
    rm test/files/docx/lorem.docx;
  fi
}
