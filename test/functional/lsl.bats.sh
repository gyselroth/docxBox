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

PATH_DOCX="test/tmp/cp_table_unordered_list_images.docx"
ERR_LOG="test/tmp/err.log"

BASE_COMMAND="docxbox lsl filename.docx searchString"

DESCRIPTION="lists all files containing given searchString"
REGEX_DESCRIPTION="lists all files containing given regular expression"

REGEX="/[0-9A-Z]\{8\}/"

SEARCH_RESULTS=(
"word/document.xml"
"word/fontTable.xml"
"word/numbering.xml"
"word/styles.xml")

REGEX_RESULT="docProps/core.xml"

@test "Exit code of \"${BASE_COMMAND}\" is zero" {
  run ${DOCXBOX_BINARY} lsl "${PATH_DOCX}" fonts
  [ "$status" -eq 0 ]
}

@test "Output of \"docxbox lsl {missing argument}\" is an error message" {
  run ${DOCXBOX_BINARY} lsl
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: DOCX filename" = "${lines[0]}" ]
}

title="Output of \"docxbox lsl filename.docx {missing argument}\" "
title+="is an error message"
@test "${title}" {
  pattern="docxBox Error - Missing argument: "
  pattern+="String or regular expression to be located"

  run ${DOCXBOX_BINARY} lsl "${PATH_DOCX}"
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]
}

@test "\"${BASE_COMMAND}\" ${DESCRIPTION}" {
  for i in "${SEARCH_RESULTS[@]}"
  do
    ${DOCXBOX_BINARY} lsl "${PATH_DOCX}" fonts | grep --count "${i}"
  done 
}

@test "\"docxbox ls filename.docx -l searchString\" ${DESCRIPTION}" {
  for i in "${SEARCH_RESULTS[@]}"
  do
    ${DOCXBOX_BINARY} ls "${PATH_DOCX}" -l fonts | grep --count "${i}"
  done
}

@test "\"docxbox ls filename.docx --locate searchString\" ${DESCRIPTION}" {
  for i in "${SEARCH_RESULTS[@]}"
  do
    ${DOCXBOX_BINARY} lsl "${PATH_DOCX}" --locate fonts | grep --count "${i}"
  done
}

@test "With \"docxbox lsl filename.docx REGEX\" ${REGEX_DESCRIPTION}" {
  ${DOCXBOX_BINARY} lsl "${PATH_DOCX}" "${REGEX}" | grep --count ${REGEX_RESULT}
}

@test "With \"docxbox ls filename.docx -l REGEX\" ${REGEX_DESCRIPTION}" {
  ${DOCXBOX_BINARY} ls "${PATH_DOCX}" -l "${REGEX}" | grep --count ${REGEX_RESULT}
}

@test "With \"docxbox ls filename.docx --locate REGEX\" ${REGEX_DESCRIPTION}" {
  ${DOCXBOX_BINARY} ls "${PATH_DOCX}" --locate "${REGEX}" | grep --count ${REGEX_RESULT}
}

@test "Output of \"docxbox lsl nonexistent.docx searchString\" is an error message" {
  run ${DOCXBOX_BINARY} lsl nonexistent.docx fonts
  [ "$status" -ne 0 ]

  ${DOCXBOX_BINARY} lsl nonexistent.docx fonts 2>&1 | tee "${ERR_LOG}"
  cat "${ERR_LOG}" | grep --count "docxBox Error - File not found:"
}

@test "Output of \"docxbox lsl wrong_file_type\" is an error message" {
  wrong_file_types=(
  "test/tmp/cp_lorem_ipsum.pdf"
  "test/tmp/cp_mock_csv.csv"
  "test/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    ${DOCXBOX_BINARY} lsl "${i}" fonts 2>&1 | tee "${ERR_LOG}"
    cat "${ERR_LOG}" | grep --count "docxBox Error - File is no ZIP archive:"
  done
}
