#!/usr/bin/env bats

load _helper

@test "Output of \"docxbox mm {missing argument}\" is an error message" {
  run $BATS_TEST_DIRNAME/docxbox mm
  [ "$status" -ne 0 ]
  [ "Missing argument: DOCX filename" = "${lines[0]}" ]
}

@test "Output of \"docxbox mm filename.docx {missing argument}\" is an error message" {
  run $BATS_TEST_DIRNAME/docxbox mm test/files/docx/table_unordered_list_images.docx
  [ "$status" -ne 0 ]
  [ "Missing argument: Meta attribute to be set" = "${lines[0]}" ]
}

@test "With \"docxbox mm filename.docx title {argument}\" the meta attribute title can be changed" {
  run $BATS_TEST_DIRNAME/docxbox mm test/files/tbl.docx title someTitle
  [ "$status" -eq 0 ]
  $BATS_TEST_DIRNAME/docxbox lsm test/files/tbl.docx | grep -c "title: someTitle"
}

@test "With \"docxbox mm filename.docx creator {argument}\" the meta attribute creator can be changed" {
  run $BATS_TEST_DIRNAME/docxbox mm test/files/tbl.docx creator someOne
  [ "$status" -eq 0 ]
  $BATS_TEST_DIRNAME/docxbox lsm test/files/tbl.docx | grep -c "creator: someOne"
}

@test "With \"docxbox mm filename.docx lastModifiedBy {argument}\" the meta attribute lastModifiedBy can be changed" {
  run $BATS_TEST_DIRNAME/docxbox mm test/files/tbl.docx lastModifiedBy someOneElse
  [ "$status" -eq 0 ]
  $BATS_TEST_DIRNAME/docxbox lsm test/files/tbl.docx | grep -c "lastModifiedBy: someOneElse"
}

@test "With \"docxbox mm filename.docx lastPrinted {argument}\" the meta attribute lastPrinted can be changed" {
  run $BATS_TEST_DIRNAME/docxbox mm test/files/tbl.docx lastPrinted 2020-02-20T10:31:00Z
  [ "$status" -eq 0 ]
  $BATS_TEST_DIRNAME/docxbox lsm test/files/tbl.docx | grep -c "lastPrinted: 2020-02-20T10:31:00Z"
}

@test "With \"docxbox mm filename.docx language {argument}\" the meta attribute language can be changed" {
  run $BATS_TEST_DIRNAME/docxbox mm test/files/tbl.docx language de-CH
  [ "$status" -eq 0 ]
  $BATS_TEST_DIRNAME/docxbox lsm test/files/tbl.docx | grep -c "language: de-CH"
}

@test "With \"docxbox mm filename.docx modified {argument}\" the meta attribute modified can be changed" {
  run $BATS_TEST_DIRNAME/docxbox mm test/files/tbl.docx modified 2019-10-10T09:21:00Z
  [ "$status" -eq 0 ]
  $BATS_TEST_DIRNAME/docxbox lsm test/files/tbl.docx | grep -c "modified: 2019-10-10T09:21:00Z"
}

@test "With \"docxbox mm filename.docx created {argument}\" the meta attribute created can be changed" {
  run $BATS_TEST_DIRNAME/docxbox mm test/files/tbl.docx created 2019-12-12T09:21:00Z
  [ "$status" -eq 0 ]
  $BATS_TEST_DIRNAME/docxbox lsm test/files/tbl.docx | grep -c "created: 2019-06-12T09:21:00Z"
}

@test "With \"docxbox mm filename.docx revision {argument}\" the meta attribute revision can be changed" {
  run $BATS_TEST_DIRNAME/docxbox mm test/files/tbl.docx revision 1
  [ "$status" -eq 0 ]
  $BATS_TEST_DIRNAME/docxbox lsm test/files/tbl.docx | grep "revision: 1"
}
