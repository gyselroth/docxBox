#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

longhand_command="docxbox ls filename.docx"
description="contains files' and directories' attributes"

attributes=(
  "length"
  "date"
  "time"
  "file")

@test "Output of \"docxbox lsj filename.docx\" ${description}" {
  for i in "${attributes[@]}"
  do
    "$BATS_TEST_DIRNAME"/docxbox lsj "${path_docx}" | grep -c "${i}"
  done
}

@test "Output of \"${longhand_command} --json\" ${description}" {
  for i in "${attributes[@]}"
  do
    "$BATS_TEST_DIRNAME"/docxbox ls "${path_docx}" --json | grep -c "${i}"
  done
}

@test "Output of \"${longhand_command} -j\" ${description}" {
  for i in "${attributes[@]}"
  do
    "$BATS_TEST_DIRNAME"/docxbox ls "${path_docx}" -j | grep -c "${i}"
  done
}

@test "Output of \"docxbox lsj filename.docx\" are contained files" {
search_values=(
"[Content_Types].xml"
"docProps/app.xml"
"docProps/core.xml"
"word/_rels/document.xml.rels"
"word/charts/chart1.xml"
"word/document.xml"
"word/fontTable.xml"
"word/media/image1.jpeg"
"word/numbering.xml"
"word/settings.xml"
"word/styles.xml")

  for i in "${search_values[@]}"
  do
    "$BATS_TEST_DIRNAME"/docxbox lsj "${path_docx}" | grep -c "${i}"
  done
}

@test "Output of \"docxbox lsj filename.docx\" contains files' date and time" {
  "$BATS_TEST_DIRNAME"/docxbox lsj "${path_docx}" | grep -c "4/11/2020"
  "$BATS_TEST_DIRNAME"/docxbox lsj "${path_docx}" | grep -c "11:3"
}

@test "Output of \"docxbox ls {missing argument}\" is an error message" {
  run "$BATS_TEST_DIRNAME"/docxbox lsj
  [ "$status" -ne 0 ]
  [ "Missing argument: DOCX filename" = "${lines[0]}" ]
}
