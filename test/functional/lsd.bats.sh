#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

VALGRIND="valgrind -v --leak-check=full\
 --log-file=test/assets/documents/other/mem-leak.log"

if $IS_VALGRIND_TEST;
then
  DOCXBOX_BINARY="${VALGRIND} $BATS_TEST_DIRNAME/../tmp/docxbox"
else
  DOCXBOX_BINARY="$BATS_TEST_DIRNAME/../tmp/docxbox"
fi

base_command="\"docxbox lsd"

path_docx="test/tmp/cp_mergefields.docx"

merge_field="MERGEFIELD"
merge_format="\* MERGEFORMAT"

@test "Exit code of ${base_command} filename.docx\" is zero" {
  run ${DOCXBOX_BINARY} lsd "${path_docx}"
  [ "$status" -eq 0 ]
}

@test "Output of ${base_command} {missing argument}\" is an error message" {
  run ${DOCXBOX_BINARY} lsd
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: Filename of DOCX to be extracted" = "${lines[0]}" ]
}

@test "With ${base_command} filename.docx\" the fields in the docx are listed" {
  ${DOCXBOX_BINARY} lsd "${path_docx}" | grep --count "${merge_field}"
  ${DOCXBOX_BINARY} lsd "${path_docx}" | grep --count "${merge_format}"
}

@test "With ${base_command} filename.docx\" the fields in the header are listed" {
  ${DOCXBOX_BINARY} lsd "${path_docx}" | grep --count "MERGEFIELD  Mergefield_Header"
}

@test "With ${base_command} filename.docx\" the fields in the footer are listed" {
  ${DOCXBOX_BINARY} lsd "${path_docx}" | grep --count "MERGEFIELD  Mergefield_Footer"
}

title="With \"docxbox ls filename.docx --fields\" "
title+="the fields in the docx are listed"
@test "$title" {
  ${DOCXBOX_BINARY} lsd "${path_docx}" | grep --count "${merge_field}"
  ${DOCXBOX_BINARY} lsd "${path_docx}" | grep --count "${merge_format}"
}

@test "With \"docxbox ls filename.docx -d\" the fields in the docx are listed" {
  ${DOCXBOX_BINARY} ls "${path_docx}" -d | grep --count "${merge_field}"
  ${DOCXBOX_BINARY} ls "${path_docx}" -d | grep --count "${merge_format}"
}

@test "Output of \"docxbox lsd nonexistent.docx\" is an error message" {
  err_log="test/tmp/err.log"

  run ${DOCXBOX_BINARY} lsd nonexistent.docx
  [ "$status" -ne 0 ]

  ${DOCXBOX_BINARY} lsd nonexistent.docx 2>&1 | tee "${err_log}"
  cat "${err_log}" | grep --count "docxBox Error - File not found:"
}

@test "Output of ${base_command} wrong_file_type\" is an error message" {
  err_log="test/tmp/err.log"
  wrong_file_types=(
  "test/tmp/cp_lorem_ipsum.pdf"
  "test/tmp/cp_mock_csv.csv"
  "test/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    ${DOCXBOX_BINARY} lsd "${i}" 2>&1 | tee "${err_log}"
    cat "${err_log}" | grep --count "docxBox Error - File is no ZIP archive:"
  done
}
