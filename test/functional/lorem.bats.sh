#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

{CMD}="docxbox lorem"

VALGRIND_LOG="test/tmp/mem-leak.log"
VALGRIND="valgrind -v --leak-check=full\
 --log-file=${VALGRIND_LOG}"

VALGRIND_ERR_PATTERN="ERROR SUMMARY: [1-9] errors from [1-9] contexts"

if $IS_VALGRIND_TEST;
then
  DOCXBOX_BINARY="${VALGRIND} $BATS_TEST_DIRNAME/../tmp/docxbox"
else
  DOCXBOX_BINARY="$BATS_TEST_DIRNAME/../tmp/docxbox"
fi

ERR_LOG="test/tmp/err.log"

@test "$BATS_TEST_NUMBER: Exit code of \"${CMD} filename.docx\" is zero" {
  local path_docx="test/tmp/cp_table_unordered_list_images.docx"

  run ${DOCXBOX_BINARY} lorem "${path_docx}"
  [ "$status" -eq 0 ]

  check_for_valgrind_error
}

@test "$BATS_TEST_NUMBER: \"${CMD} {missing argument}\" prints an error message" {
  local pattern="docxBox Error - Missing argument: Filename of DOCX to be extracted"

  run ${DOCXBOX_BINARY} lorem
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]

  check_for_valgrind_error
}

@test "$BATS_TEST_NUMBER: \"${CMD} filename.docx\" replaces all text by dummy text" {
  local path_docx="test/tmp/cp_table_unordered_list_images.docx"
  local pattern="Culpa ad eiusmod"

  run ${DOCXBOX_BINARY} lorem "${path_docx}"
  [ "$status" -eq 0 ]

  check_for_valgrind_error

  ${DOCXBOX_BINARY} txt "${path_docx}" | grep --invert-match --count "${pattern}"
}

@test "$BATS_TEST_NUMBER: \"${CMD} filename.docx newFilename.docx\" replaces all text by dummy text and saves it to new file" {
  local path_docx_1="test/tmp/cp_table_unordered_list_images.docx"
  local path_docx_2="test/tmp/lorem.docx"

  ${DOCXBOX_BINARY} lorem "${path_docx_1}" "${path_docx_2}"

  check_for_valgrind_error

  ls test/tmp | grep -c lorem.docx
}

@test "$BATS_TEST_NUMBER: \"${CMD} nonexistent.docx\" prints an error message" {
  run ${DOCXBOX_BINARY} lorem nonexistent.docx
  [ "$status" -ne 0 ]

  check_for_valgrind_error

  ${DOCXBOX_BINARY} lorem nonexistent.docx 2>&1 | tee "${ERR_LOG}"
  cat "${ERR_LOG}" | grep --count "docxBox Error - File not found:"
}

@test "$BATS_TEST_NUMBER: \"${CMD} wrong_file_type\" prints an error message" {
  local pattern="docxBox Error - File is no ZIP archive:"
  local wrong_file_types=(
  "test/tmp/cp_lorem_ipsum.pdf"
  "test/tmp/cp_mock_csv.csv"
  "test/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    ${DOCXBOX_BINARY} lorem "${i}" 2>&1 | tee "${ERR_LOG}"
    check_for_valgrind_error
    cat "${ERR_LOG}" | grep --count "${pattern}"
  done
}
