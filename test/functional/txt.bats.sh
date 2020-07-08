#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

VALGRIND_LOG="test/tmp/mem-leak.log"
VALGRIND="valgrind -v --leak-check=full\
 --log-file=${VALGRIND_LOG}"

VALGRIND_ERR_PATTERN="ERROR SUMMARY: [0-9] errors from [0-9] contexts"

if $IS_VALGRIND_TEST;
then
  DOCXBOX_BINARY="${VALGRIND} $BATS_TEST_DIRNAME/../tmp/docxbox"
else
  DOCXBOX_BINARY="$BATS_TEST_DIRNAME/../tmp/docxbox"
fi

PATH_DOCX="test/tmp/cp_table_unordered_list_images.docx"
ERR_LOG="test/tmp/err.log"

BASE_COMMAND="docxbox txt filename.docx"
APPENDIX="is the segmented plain text from given file"

@test "Case 1: Output of \"docxbox txt {missing argument}\" is an error message" {
  run ${DOCXBOX_BINARY} txt
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: Filename of DOCX to be extracted" = "${lines[0]}" ]

  check_for_valgrind_error
}

@test "Case 2: Output of \"${BASE_COMMAND}\" is the the plain text from given file" {
  ${DOCXBOX_BINARY} txt "${PATH_DOCX}" | grep --count "Officia"

  check_for_valgrind_error
}

@test "Case 3: Output of \"${BASE_COMMAND} -s \" ${APPENDIX}" {
  segmented=$(${DOCXBOX_BINARY} txt "${PATH_DOCX}" -s | wc --lines)
  non_segmented=$(${DOCXBOX_BINARY} txt "${PATH_DOCX}" | wc --lines)

  check_for_valgrind_error

  (( ${segmented} > ${non_segmented} ))

}

@test "Case 4: Output of \"${BASE_COMMAND} --segments \" ${APPENDIX}" {
  segmented=$(${DOCXBOX_BINARY} txt "${PATH_DOCX}" --segments | wc --lines)
  non_segmented=$(${DOCXBOX_BINARY} txt "${PATH_DOCX}" | wc --lines)

  check_for_valgrind_error

  (( ${segmented} > ${non_segmented} ))
}

@test "Case 5: Output of \"docxbox txt nonexistent.docx\" is an error message" {
  run ${DOCXBOX_BINARY} txt nonexistent.docx
  [ "$status" -ne 0 ]

  check_for_valgrind_error

  ${DOCXBOX_BINARY} txt nonexistent.docx 2>&1 | tee "${ERR_LOG}"
  cat "${ERR_LOG}" | grep --count "docxBox Error - File not found:"
}

@test "Case 6: Output of \"docxbox txt wrong_file_type\" is an error message" {
  pattern="docxBox Error - File is no ZIP archive:"
  wrong_file_types=(
  "test/tmp/cp_lorem_ipsum.pdf"
  "test/tmp/cp_mock_csv.csv"
  "test/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    ${DOCXBOX_BINARY} txt "${i}" 2>&1 | tee "${ERR_LOG}"
    check_for_valgrind_error
    cat "${ERR_LOG}" | grep --count "${pattern}"
  done
}

check_for_valgrind_error() {
  if $IS_VALGRIND_TEST; then
    cat "${VALGRIND_LOG}" | grep --count --invert-match "${VALGRIND_ERR_PATTERN}"
  fi
}
