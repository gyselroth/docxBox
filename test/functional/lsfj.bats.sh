#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

docxbox=""$BATS_TEST_DIRNAME"/docxbox"
path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

base_command="docxbox lsfj filename.docx"
longhand_command="docxbox lsf filename.docx"

long_description="contains files' and directories' attributes"

attributes=(
  "font"
  "altName"
  "charset"
  "family"
  "pitch")

@test "Exit code of \"docxbox ls filename.docx\" is zero" {
  run "${docxbox}" lsfj "${path_docx}"
  [ "$status" -eq 0 ]
}

@test "Output of \"docxbox lsfj {missing argument}\" is an error message" {
  run "${docxbox}" lsfj
  [ "$status" -ne 0 ]
  [ "Missing argument: Filename of DOCX to be extracted" = "${lines[0]}" ]
}

@test "Output of \"${base_command}\" ${long_description}" {
  for i in "${attributes[@]}"
  do
    "${docxbox}" lsfj "${path_docx}" | grep --count "${i}"
  done
}

@test "Output of \"${longhand_command} --json\" ${long_description}" {
  for i in "${attributes[@]}"
  do
    "${docxbox}" lsf "${path_docx}" --json | grep --count "${i}"
  done
}

@test "Output of \"${longhand_command} -j\" ${long_description}" {
  for i in "${attributes[@]}"
  do
    "${docxbox}" lsf "${path_docx}" -j | grep --count "${i}"
  done
}

longhand="--fonts --json"
@test "Output of \"docxbox ls filename.docx ${longhand}\" ${long_description}" {
  for i in "${attributes[@]}"
  do
    "${docxbox}" ls "${path_docx}" ${longhand} | grep --count "${i}"
  done
}

@test "Output of \"docxbox ls filename.docx -fj\" ${long_description}" {
  for i in "${attributes[@]}"
  do
    "${docxbox}" ls "${path_docx}" -fj | grep --count "${i}"
  done
}

@test "Output of \"${base_command}\" contains fontfile-filename" {
  "${docxbox}" lsfj "${path_docx}" | grep --count "fontTable.xml"
}

@test "Output of \"${base_command}\" contains font names" {
  font_names=(
  "Times New Roman"
  "Symbol"
  "Arial"
  "Liberation Serif"
  "OpenSymbol"
  "Liberation Sans"
  "DejaVu Sans"
  "OpenSymbol")

  for i in "${font_names[@]}"
  do
    "${docxbox}" lsfj "${path_docx}" | grep --count "${i}"
  done
}

@test "Output of \"${base_command}\" can contain alternative font names" {
  "${docxbox}" lsfj "${path_docx}" | grep --count "Arial Unicode MS"
}

@test "Output of \"${base_command}\" contains font-charSets" {
  "${docxbox}" lsfj "${path_docx}" | grep --count "00"
}

@test "Output of \"${base_command}\" contains font-family" {
  font_family=(
  "roman"
  "swiss"
  "auto")

  for i in "${font_family[@]}"
  do
    "${docxbox}" lsfj "${path_docx}" | grep --count "${i}"
  done
}

@test "Output of \"${base_command}\" contains font-pitch" {
  "${docxbox}" lsfj "${path_docx}" | grep --count "variable"
  "${docxbox}" lsfj "${path_docx}" | grep --count "default"
}

@test "Output of \"docxbox lsfj wrong_file_type\" is an error message" {
  err_log="test/functional/tmp/err.log"
  wrong_file_types=(
  "test/functional/tmp/cp_lorem_ipsum.pdf"
  "test/functional/tmp/cp_mock_csv.csv"
  "test/functional/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    "${docxbox}" lsfj "${i}" 2>&1 | tee "${err_log}"
    cat "${err_log}" | grep --count "Not a valid DOX (ZIP) archive:"
  done
}
