#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

base_command="docxbox lsl filename.docx searchString"

description="lists all files containing given searchString"
regex_description="lists all files containing given regular expression"

regex="/[0-9A-Z]\{8\}/"

search_results=(
"word/document.xml"
"word/fontTable.xml"
"word/numbering.xml"
"word/styles.xml")

regex_result="docProps/core.xml"

@test "Exit code of \"${base_command}\" is zero" {
  run "$BATS_TEST_DIRNAME"/docxbox lsl "${path_docx}" fonts
  [ "$status" -eq 0 ]
}

@test "Output of \"docxbox lsl {missing argument}\" is an error message" {
  run "$BATS_TEST_DIRNAME"/docxbox lsl
  [ "$status" -ne 0 ]
  [ "Missing argument: DOCX filename" = "${lines[0]}" ]
}

@test "Output of \"docxbox lsl filename.docx {missing argument}\" is an error message" {
  pattern="Missing argument: String or regular expression to be located"

  run "$BATS_TEST_DIRNAME"/docxbox lsl path_docx
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]
}

@test "\"${base_command}\" ${description}" {
  for i in "${search_results[@]}"
  do
    "$BATS_TEST_DIRNAME"/docxbox lsl "${path_docx}" fonts | grep -c "${i}"
  done 
}

@test "\"docxbox ls filename.docx -l searchString\" ${description}" {
  for i in "${search_results[@]}"
  do
    "$BATS_TEST_DIRNAME"/docxbox ls "${path_docx}" fonts -l | grep -c "${i}"
  done
}

@test "\"docxbox ls filename.docx --locate searchString\" ${description}" {
  for i in "${search_results[@]}"
  do
    "$BATS_TEST_DIRNAME"/docxbox lsl "${path_docx}" fonts --locate | grep -c "${i}"
  done
}

@test "\"docxbox lsl filename.docx regex\" ${regex_description}" {
  "$BATS_TEST_DIRNAME"/docxbox lsl "${path_docx}" ${regex} | grep -c ${regex_result}
}

@test "\"docxbox ls filename.docx -l regex\" ${regex_description}" {
  "$BATS_TEST_DIRNAME"/docxbox ls "${path_docx}" -l ${regex} | grep -c ${regex_result}
}

@test "\"docxbox ls filename.docx --locate regex\" ${regex_description}" {
  "$BATS_TEST_DIRNAME"/docxbox ls "${path_docx}" --locate ${regex} | grep -c ${regex_result}
}
