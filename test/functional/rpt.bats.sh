#!/usr/bin/env bats

load _helper

@test "Output of \"docxbox rpt {missing filename}\" is an error message" {
  run "$BATS_TEST_DIRNAME"/docxbox rpt
  [ "$status" -ne 0 ]
  [ "Missing argument: DOCX Filename" = "${lines[0]}" ]
}

@test "Output of \"docxbox rpt filename.docx {missing arguments}\" is an error message" {
  run $BATS_TEST_DIRNAME/docxbox rpt test/files/docx/table_unordered_list_images.docx
  [ "$status" -ne 0 ]
  [ "Missing argument: String to be found (and replaced)" = "${lines[0]}" ]
}

@test "Output of \"docxbox rpt filename.docx stringToBeReplaced {missing argument}\" is an error message" {
  run $BATS_TEST_DIRNAME/docxbox rpt test/files/docx/table_unordered_list_images.docx toBeReplaced
  [ "$status" -ne 0 ]
  [ "Missing argument: Replacement string" = "${lines[0]}" ]
}

@test "With \"docxbox rpt filename.docx stringToBeReplaced replacementString\" the stringToBeReplaced gets replaced" {
  run $BATS_TEST_DIRNAME/docxbox rpt test/files/docx/tbl.docx Lorem Dorem
  [ "$status" -eq 0 ]
  $BATS_TEST_DIRNAME/docxbox txt test/files/docx/tbl.docx | grep -c Dorem
}

@test "With \"docxbox rpt filename.docx stringToBeReplaced replacementString newFile.docx\" the stringToBeReplaced gets replaced and is saved to new file" {
  run $BATS_TEST_DIRNAME/docxbox rpt test/files/docx/tbl.docx Lorem Dorem test/files/docx/replacedString.docx
  [ "$status" -eq 0 ]
  $BATS_TEST_DIRNAME/docxbox txt test/files/docx/replacedString.docx | grep -c Dorem
  if [ -f test/files/docx/replacedString.docx ]; then
    rm test/files/docx/replacedString.docx;
  fi
}
