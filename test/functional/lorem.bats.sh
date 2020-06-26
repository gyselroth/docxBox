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

base_command="docxbox lorem filename.docx"

@test "Exit code of \"${base_command}\" is zero" {
  path_docx="test/tmp/cp_table_unordered_list_images.docx"

  run ${DOCXBOX_BINARY} lorem "${path_docx}"
  [ "$status" -eq 0 ]
}

@test "Output of \"docxbox lorem {missing argument}\" is an error message" {
  pattern="docxBox Error - Missing argument: Filename of DOCX to be extracted"

  run ${DOCXBOX_BINARY} lorem
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]
}

@test "With \"${base_command}\" text gets replaced by dummy text" {
  path_docx="test/tmp/cp_table_unordered_list_images.docx"
  pattern="Culpa ad eiusmod"

  run ${DOCXBOX_BINARY} lorem "${path_docx}"
  [ "$status" -eq 0 ]

  ${DOCXBOX_BINARY} txt "${path_docx}" | grep --invert-match --count "${pattern}"
}

title="With \"${base_command} newFilename.docx\" "
title+="text gets replaced by dummy text and is saved to new file"
@test "${title}" {
  path_docx_1="test/tmp/cp_table_unordered_list_images.docx"
  path_docx_2="test/tmp/lorem.docx"

  ${DOCXBOX_BINARY} lorem "${path_docx_1}" "${path_docx_2}"
  ls test/tmp | grep -c lorem.docx
}

@test "Output of \"docxbox lorem nonexistent.docx\" is an error message" {
  err_log="test/tmp/err.log"

  run ${DOCXBOX_BINARY} lorem nonexistent.docx
  [ "$status" -ne 0 ]

  ${DOCXBOX_BINARY} lorem nonexistent.docx 2>&1 | tee "${err_log}"
  cat "${err_log}" | grep --count "docxBox Error - File not found:"
}

@test "Output of \"docxbox lorem wrong_file_type\" is an error message" {
  pattern="docxBox Error - File is no ZIP archive:"
  err_log="test/tmp/err.log"
  wrong_file_types=(
  "test/tmp/cp_lorem_ipsum.pdf"
  "test/tmp/cp_mock_csv.csv"
  "test/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    ${DOCXBOX_BINARY} lorem "${i}" 2>&1 | tee "${err_log}"
    cat "${err_log}" | grep --count "${pattern}"
  done
}
