#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

VALGRIND="valgrind -v --leak-check=full\
 --log-file=test/tmp/mem-leak.log"

if $IS_VALGRIND_TEST;
then
  DOCXBOX_BINARY="${VALGRIND} $BATS_TEST_DIRNAME/../tmp/docxbox"
else
  DOCXBOX_BINARY="$BATS_TEST_DIRNAME/../tmp/docxbox"
fi

PATH_DOCX="test/tmp/cp_table_unordered_list_images.docx"
ERR_LOG="test/tmp/err.log"

BASE_COMMAND="docxbox lsi filename.docx"

@test "Exit code of \"${BASE_COMMAND}\" is zero" {
  run ${DOCXBOX_BINARY} lsi "${PATH_DOCX}"
  [ "$status" -eq 0 ]
}

@test "Output of \"docxbox lsi {missing argument}\" is an error message" {
  pattern="docxBox Error - Missing argument: Filename of DOCX to be extracted"

  run ${DOCXBOX_BINARY} lsi
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]
}

@test "Output of \"${BASE_COMMAND}\" contains files' and directories' attributes" {
  attributes=(
  "Length"
  "Date"
  "Time"
  "Name")

  for i in "${attributes[@]}"
  do
    ${DOCXBOX_BINARY} ls "${PATH_DOCX}" | grep --count "${i}"
  done
}

@test "Output of \"${BASE_COMMAND}\" is contained images" {
  run ${DOCXBOX_BINARY} lsi "${PATH_DOCX}"
  [ "$status" -eq 0 ]
  ${DOCXBOX_BINARY} lsi "${PATH_DOCX}" | grep --count "image2.jpeg"
}

@test "Output of \"docxbox ls filename.docx -i\" is contained images" {
  ${DOCXBOX_BINARY} ls "${PATH_DOCX}" -i | grep --count "image2.jpeg"
}

@test "Output of \"docxbox ls filename.docx --images\" is contained images" {
  ${DOCXBOX_BINARY} ls "${PATH_DOCX}" --images | grep --count "image2.jpeg"
}

@test "Output of \"docxbox lsi nonexistent.docx\" is an error message" {
  run "$BATS_TEST_DIRNAME"/docxbox lsi nonexistent.docx
  [ "$status" -ne 0 ]

  ${DOCXBOX_BINARY} lsi nonexistent.docx 2>&1 | tee "${ERR_LOG}"
  cat "${ERR_LOG}" | grep --count "docxBox Error - File not found:"
}

@test "Output of \"docxbox lsi wrong_file_type\" is an error message" {
  wrong_file_types=(
  "test/tmp/cp_lorem_ipsum.pdf"
  "test/tmp/cp_mock_csv.csv"
  "test/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    ${DOCXBOX_BINARY} lsi "${i}" 2>&1 | tee "${ERR_LOG}"
    cat "${ERR_LOG}" | grep --count "docxBox Error - File is no ZIP archive:"
  done
}
