#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

docxbox=""$BATS_TEST_DIRNAME"/docxbox"

base_command="\"docxbox ls filename.docx"
path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

@test "Exit code of ${base_command}\" is zero" {
  run "${docxbox}" ls "${path_docx}"
  [ "$status" -eq 0 ]
}

@test "Output of \"docxbox ls {missing argument}\" is an error message" {
  run "${docxbox}" ls
  [ "$status" -ne 0 ]
  [ "Missing argument: DOCX filename" = "${lines[0]}" ]
}

@test "Output of ${base_command}\" contains files' and directories' attributes" {
  attributes=(
  "Length"
  "Date"
  "Time"
  "Name")

  for i in "${attributs[@]}"
  do
    "${docxbox}" ls "${path_docx}" | grep --count "${i}"
  done
}

@test "Output of ${base_command}\" is contained files" {
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
    "${docxbox}" ls "${path_docx}" | grep --count "${i}"
  done
}

@test "Output of ${base_command}\" contains amount of contained files" {
  "${docxbox}" ls "${path_docx}" | grep --count '11 files'
}

@test "Output of ${base_command}\" contains files' date and time" {
  "${docxbox}" ls "${path_docx}" | grep --count "4/11/2020"
  "${docxbox}" ls "${path_docx}" | grep --count "11:3"
}

long_description="contains files with the given file ending"
@test "Output of ${base_command} *.file-ending\" ${long_description}" {
  "${docxbox}" ls "${path_docx}" *.jpeg | grep --count "image1.jpeg"
  "${docxbox}" ls "${path_docx}" *.xml | grep --count "9 files"
}
