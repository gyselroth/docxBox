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

SEARCH_RESULTS=(
"\"file\":\"word/document.xml\""
"\"file\":\"word/fontTable.xml\""
"\"file\":\"word/numbering.xml\""
"\"file\":\"word/styles.xml\"")

@test "Case 1: Output of \"docxbox lslj {missing argument}\" ${error_message}" {
  run ${DOCXBOX_BINARY} lslj
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: DOCX filename" = "${lines[0]}" ]
  
  check_for_valgrind_error
}

@test "Case 2: Output of \"docxbox lslj filename.docx {missing argument} ${error_message}" {
  pattern="docxBox Error - Missing argument: "
  pattern+="String or regular expression to be located"

  run ${DOCXBOX_BINARY} lslj "${PATH_DOCX}"
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]
  
  check_for_valgrind_error
}

@test "Case 3: \"docxbox lslj filename.docx searchString\" ${description}" {
  for i in "${SEARCH_RESULTS[@]}"
  do
    ${DOCXBOX_BINARY} lslj "${PATH_DOCX}" fonts | grep -c "${i}"
    check_for_valgrind_error
  done
}

@test "Case 4: \"docxbox lsl filename.docx -j searchString\" ${description}" {
  for i in "${SEARCH_RESULTS[@]}"
  do
    ${DOCXBOX_BINARY} lsl "${PATH_DOCX}" -j fonts | grep -c "${i}"
    check_for_valgrind_error
  done
}

@test "Case 5: \"docxbox lsl filename.docx --json searchString\" ${description}" {
  for i in "${SEARCH_RESULTS[@]}"
  do
    ${DOCXBOX_BINARY} lsl "${PATH_DOCX}" --json fonts | grep -c "${i}"
    check_for_valgrind_error
  done
}

@test "Case 6: \"docxbox ls filename.docx -lj searchString\" ${description}" {
  for i in "${SEARCH_RESULTS[@]}"
  do
    ${DOCXBOX_BINARY} ls "${PATH_DOCX}" -lj fonts | grep -c "${i}"
    check_for_valgrind_error
  done
}

@test "Case 7: \"docxbox ls filename.docx --locate -j searchString\" ${description}" {
  for i in "${SEARCH_RESULTS[@]}"
  do
    ${DOCXBOX_BINARY} ls "${PATH_DOCX}" --locate -j fonts | grep -c "${i}"
    check_for_valgrind_error
  done
}

@test "Case 8: \"docxbox ls filename.docx --locate --json searchString\" ${description}" {
  for i in "${SEARCH_RESULTS[@]}"
  do
    ${DOCXBOX_BINARY} ls "${PATH_DOCX}" --locate --json fonts | grep -c "${i}"
    check_for_valgrind_error
  done
}

@test "Case 9: Output of \"docxbox lslj nonexistent.docx searchString\" is an error message" {
  run ${DOCXBOX_BINARY} lslj nonexistent.docx fonts
  [ "$status" -ne 0 ]

  check_for_valgrind_error

  ${DOCXBOX_BINARY} lslj nonexistent.docx fonts 2>&1 | tee "${ERR_LOG}"
  cat "${ERR_LOG}" | grep --count "docxBox Error - File not found:"
}

@test "Case 10: Output of \"docxbox lslj wrong_file_type searchString\" ${error_message}" {
  wrong_file_types=(
  "test/tmp/cp_lorem_ipsum.pdf"
  "test/tmp/cp_mock_csv.csv"
  "test/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    ${DOCXBOX_BINARY} lslj "${i}" fonts 2>&1 | tee "${ERR_LOG}"
    check_for_valgrind_error
    cat "${ERR_LOG}" | grep --count "docxBox Error - File is no ZIP archive:"
  done
}

check_for_valgrind_error() {
  if $IS_VALGRIND_TEST; then
    cat "${VALGRIND_LOG}" | grep --count --invert-match "${VALGRIND_ERR_PATTERN}"
  fi
}
