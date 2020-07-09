#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

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

PATH_DOCX="test/tmp/cp_table_unordered_list_images.docx"
ERR_LOG="test/tmp/err.log"

BASE_COMMAND="docxbox lsij filename.docx"
DESCRIPTION="are contained images as JSON"

@test "Case 1: Exit code of \"${BASE_COMMAND}\" is zero" {
  run ${DOCXBOX_BINARY} lsij "${PATH_DOCX}"
  [ "$status" -eq 0 ]

  check_for_valgrind_error
}

@test "Case 2: Output of \"docxbox lsij {missing argument}\" is an error message" {
  run ${DOCXBOX_BINARY} lsij
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: DOCX filename" = "${lines[0]}" ]

  check_for_valgrind_error
}

@test "Case 3: Output of \"${BASE_COMMAND}\" is contained images as JSON" {
  ${DOCXBOX_BINARY} lsij "${PATH_DOCX}" | grep --count "image2.jpeg"

  check_for_valgrind_error
}

@test "Case 4: Output of \"docxbox lsi filename.docx --json\" ${DESCRIPTION}" {
  ${DOCXBOX_BINARY} lsi "${PATH_DOCX}" --json | grep --count "image2.jpeg"

  check_for_valgrind_error
}

@test "Case 5: Output of \"docxbox lsi filename.docx -j\" ${DESCRIPTION}" {
  ${DOCXBOX_BINARY} lsi "${PATH_DOCX}" -j | grep --count "image2.jpeg"

  check_for_valgrind_error
}

@test "Case 6: Output of \"docxbox ls filename.docx -ij\" ${DESCRIPTION}" {
  ${DOCXBOX_BINARY} lsi "${PATH_DOCX}" -ij | grep --count "image2.jpeg"

  check_for_valgrind_error
}

@test "Case 7: Output of \"docxbox ls filename.docx --images --json\" ${DESCRIPTION}" {
  ${DOCXBOX_BINARY} ls "${PATH_DOCX}" --images --json | grep --count "image2.jpeg"

  check_for_valgrind_error
}

@test "Case 8: Output of \"docxbox lsij nonexistent.docx\" is an error message" {
  run ${DOCXBOX_BINARY} lsij nonexistent.docx
  [ "$status" -ne 0 ]

  check_for_valgrind_error

  ${DOCXBOX_BINARY} lsij nonexistent.docx 2>&1 | tee "${ERR_LOG}"
  cat "${ERR_LOG}" | grep --count "docxBox Error - File not found:"
}

@test "Case 9: Output of \"docxbox lsij wrong_file_type\" is an error message" {
  wrong_file_types=(
  "test/tmp/cp_lorem_ipsum.pdf"
  "test/tmp/cp_mock_csv.csv"
  "test/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    ${DOCXBOX_BINARY} lsij "${i}" 2>&1 | tee "${ERR_LOG}"
    check_for_valgrind_error
    cat "${ERR_LOG}" | grep --count "docxBox Error - File is no ZIP archive:"
  done
}

title_first_char="Case 9: First char of the output of \"./docxbox lsij \
filename.docx\"'s JSON is a \"[\""
@test "${title_first_char}" {
  ${DOCXBOX_BINARY} lsij "${PATH_DOCX}" | grep "^[[]"
}

title_last_char="Case 10: Last char of the output of \"./docxbox lsij \
filename.docx\"'s JSON is a \"]\""
@test "${title_last_char}" {
  ${DOCXBOX_BINARY} lsij "${PATH_DOCX}" | grep "[]]$"
}

@test "Case 11: Amount opening and closing brackets \"[]\" must match" {
  amount_opening=$(${DOCXBOX_BINARY} lsij "${PATH_DOCX}" | grep --count "\[")
  amount_closing=$(${DOCXBOX_BINARY} lsij "${PATH_DOCX}" | grep --count "\]")

  (( amount_opening = amount_closing ))
}

@test "Case 12: Amount opening and closing brackets \"{}\" must match" {
  amount_opening=$(${DOCXBOX_BINARY} lsij "${PATH_DOCX}" | grep --count "\{")
  amount_closing=$(${DOCXBOX_BINARY} lsij "${PATH_DOCX}" | grep --count "\}")

  (( amount_opening = amount_closing ))
}

check_for_valgrind_error() {
  if $IS_VALGRIND_TEST; then
    n=$(cat "${VALGRIND_LOG}" | grep --count "${VALGRIND_ERR_PATTERN}" || /bin/true)
    if [ "$n" -eq 0 ]; then
      return 0
    else
      echo "There was a possible memory leak" >&2
      return 1
    fi
  fi
}
