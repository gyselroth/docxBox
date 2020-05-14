#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

search_results=(
"\"file\":\"word/document.xml\""
"\"file\":\"word/fontTable.xml\""
"\"file\":\"word/numbering.xml\""
"\"file\":\"word/styles.xml\"")

@test "Output of \"docxbox lslj {missing argument}\" is an error message" {
  run "$BATS_TEST_DIRNAME"/docxbox lslj
  [ "$status" -ne 0 ]
  [ "Missing argument: DOCX filename" = "${lines[0]}" ]
}

@test "Output of \"docxbox lslj filename.docx {missing argument} is an error message" {
  run "$BATS_TEST_DIRNAME"/docxbox lslj $path_docx
  [ "$status" -ne 0 ]
  [ "Missing argument: String or regular expression to be located" = "${lines[0]}" ]
}

@test "\"docxbox lslj filename.docx searchString\" ${description}" {
  for i in ${search_results[@]}
  do
    "$BATS_TEST_DIRNAME"/docxbox lslj $path_docx fonts | grep -c $i
  done
}

@test "\"docxbox lsl filename.docx -j searchString\" ${description}" {
  for i in ${search_results[@]}
  do
    "$BATS_TEST_DIRNAME"/docxbox lsl $path_docx -j fonts | grep -c $i
  done
}

@test "\"docxbox lsl filename.docx --json searchString\" ${description}" {
  for i in ${search_results[@]}
  do
    "$BATS_TEST_DIRNAME"/docxbox lsl $path_docx --json fonts | grep -c $i
  done
}

@test "\"docxbox ls filename.docx -lj searchString\" ${description}" {
  for i in ${search_results[@]}
  do
    "$BATS_TEST_DIRNAME"/docxbox ls $path_docx -lj fonts | grep -c $i
  done
}

@test "\"docxbox ls filename.docx --locate -j searchString\" ${description}" {
  for i in ${search_results[@]}
  do
    "$BATS_TEST_DIRNAME"/docxbox ls $path_docx --locate -j fonts | grep -c $i
  done
}

@test "\"docxbox ls filename.docx --locate --json searchString\" ${description}" {
for i in ${search_results[@]}
  do
    "$BATS_TEST_DIRNAME"/docxbox ls $path_docx --locate --json fonts | grep -c $i
  done
}
