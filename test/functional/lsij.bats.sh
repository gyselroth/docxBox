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

BASE_COMMAND="docxbox lsij filename.docx"
DESCRIPTION="are contained images as JSON"

@test "Exit code of \"${BASE_COMMAND}\" is zero" {
  run ${DOCXBOX_BINARY} lsij "${PATH_DOCX}"
  [ "$status" -eq 0 ]
}

@test "Output of \"docxbox lsij {missing argument}\" is an error message" {
  run ${DOCXBOX_BINARY} lsij
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: DOCX filename" = "${lines[0]}" ]
}

@test "Output of \"${BASE_COMMAND}\" is contained images as JSON" {
  ${DOCXBOX_BINARY} lsij "${PATH_DOCX}" | grep --count "image2.jpeg"
}

@test "Output of \"docxbox lsi filename.docx --json\" ${DESCRIPTION}" {
  ${DOCXBOX_BINARY} lsi "${PATH_DOCX}" --json | grep --count "image2.jpeg"
}

@test "Output of \"docxbox lsi filename.docx -j\" ${DESCRIPTION}" {
  ${DOCXBOX_BINARY} lsi "${PATH_DOCX}" -j | grep --count "image2.jpeg"
}

@test "Output of \"docxbox ls filename.docx -ij\" ${DESCRIPTION}" {
  ${DOCXBOX_BINARY} lsi "${PATH_DOCX}" -ij | grep --count "image2.jpeg"
}

@test "Output of \"docxbox ls filename.docx --images --json\" ${DESCRIPTION}" {
  ${DOCXBOX_BINARY} ls "${PATH_DOCX}" --images --json | grep --count "image2.jpeg"
}

@test "Output of \"docxbox lsij nonexistent.docx\" is an error message" {
  run "$BATS_TEST_DIRNAME"/docxbox lsij nonexistent.docx
  [ "$status" -ne 0 ]

  ${DOCXBOX_BINARY} lsij nonexistent.docx 2>&1 | tee "${ERR_LOG}"
  cat "${ERR_LOG}" | grep --count "docxBox Error - File not found:"
}

@test "Output of \"docxbox lsij wrong_file_type\" is an error message" {
  wrong_file_types=(
  "test/tmp/cp_lorem_ipsum.pdf"
  "test/tmp/cp_mock_csv.csv"
  "test/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    ${DOCXBOX_BINARY} lsij "${i}" 2>&1 | tee "${ERR_LOG}"
    cat "${ERR_LOG}" | grep --count "docxBox Error - File is no ZIP archive:"
  done
}
