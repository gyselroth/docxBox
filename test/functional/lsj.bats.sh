#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

docxbox=""$BATS_TEST_DIRNAME"/docxbox"

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
    "${docxbox}" lsj "${path_docx}" | grep --count "${i}"
  done
}

@test "Output of \"${longhand_command} --json\" ${description}" {
  for i in "${attributes[@]}"
  do
    "${docxbox}" ls "${path_docx}" --json | grep --count "${i}"
  done
}

@test "Output of \"${longhand_command} -j\" ${description}" {
  for i in "${attributes[@]}"
  do
    "${docxbox}" ls "${path_docx}" -j | grep --count "${i}"
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
    "${docxbox}" lsj "${path_docx}" | grep --count "${i}"
  done
}

@test "Output of \"docxbox lsj filename.docx\" contains files' date and time" {
  "${docxbox}" lsj "${path_docx}" | grep --count "5/14/2020"
  "${docxbox}" lsj "${path_docx}" | grep --count "23:58"
}

@test "Output of \"docxbox ls {missing argument}\" is an error message" {
  run "${docxbox}" lsj
  [ "$status" -ne 0 ]
  [ "Missing argument: DOCX filename" = "${lines[0]}" ]
}
