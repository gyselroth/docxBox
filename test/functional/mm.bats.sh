#!/usr/bin/env bats

load _helper

#cp test/files/table_unordered_list_images.docx test/files/tbl.docx

@test "Output of \"docxbox mm {missing argument}\" is an error message" {
  run $BATS_TEST_DIRNAME/docxbox mm
  [ "$status" -ne 0 ]
  [ "Missing argument: Meta attribute to be set" = "${lines[0]}" ]
}

@test "Output of \"docxbox mm filename.docx {missing argument}\" is an error message" {
  run $BATS_TEST_DIRNAME/docxbox mm test/files/table_unordered_list_images.docx
  [ "$status" -ne 0 ]
  [ "Missing argument: Meta attribute to be set" = "${lines[0]}" ]
}

@test "With \"docxbox mm filename.docx {meta-attribute}\" the meta attribute revision can be changeg" {
  run $BATS_TEST_DIRNAME/docxbox mm test/files/tbl.docx revision 1
  [ "$status" -eq 0 ]
  run $BATS_TEST_DIRNAME/docxbox lsm test/files/tbl.docx
  [ "$status" -eq 0 ]
  [ "revision: 1" = "${lines[3]}" ]
}

#teardown() {
#rm test/files/tbl.docx
#}
