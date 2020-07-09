#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

CMD="docxbox ls"

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
PATH_NEW_DOCX="test/tmp/changedFile.docx"

ERR_LOG="test/tmp/err.log"

@test "$BATS_TEST_NUMBER: Exit code of \"${CMD}\" is zero" {
  run ${DOCXBOX_BINARY} ls "${PATH_DOCX}"
  [ "$status" -eq 0 ]

  check_for_valgrind_error
}

@test "${BATS_TEST_NUMBER}: \"${CMD} {missing argument}\" prints an error message" {
  run ${DOCXBOX_BINARY} ls
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: DOCX filename" = "${lines[0]}" ]

  check_for_valgrind_error
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx\" displays files' and directories' attributes" {
  local attributes=(
  "Length"
  "Date"
  "Time"
  "Name")

  for i in "${attributs[@]}"
  do
    ${DOCXBOX_BINARY} ls "${PATH_DOCX}" | grep --count "${i}"
    check_for_valgrind_error
  done
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx\" displays contained files" {
local search_values=(
  "[Content_Types].xml"
  "_rels/.rels"
  "word/_rels/document.xml.rels"
  "word/document.xml"
  "word/theme/theme1.xml"
  "word/media/image1.png"
  "word/media/image2.jpeg"
  "word/settings.xml"
  "word/webSettings.xml"
  "word/styles.xml"
  "word/numbering.xml"
  "docProps/core.xml"
  "word/fontTable.xml"
  "docProps/app.xml")

  for i in "${search_values[@]}"
  do
    ${DOCXBOX_BINARY} ls "${PATH_DOCX}" | grep --count "${i}"
    check_for_valgrind_error
  done
}

@test "${BATS_TEST_NUMBER}: \"${CMD}\" displays amount of contained files" {
  ${DOCXBOX_BINARY} ls "${PATH_DOCX}" | grep --count '14 files'

  check_for_valgrind_error
}

@test "${BATS_TEST_NUMBER}: \"${CMD}\" displays files' date and time" {
  ${DOCXBOX_BINARY} ls "${PATH_DOCX}" | grep --count "7/3/2020"
  ${DOCXBOX_BINARY} ls "${PATH_DOCX}" | grep --count "7/3/2020"

  check_for_valgrind_error
}

@test "${BATS_TEST_NUMBER}: \"${CMD} *.file-ending\" displays files with the given file ending" {
  ${DOCXBOX_BINARY} ls "${PATH_DOCX}" *.jpeg | grep --count "image2.jpeg"
  ${DOCXBOX_BINARY} ls "${PATH_DOCX}" *.xml | grep --count "10 files"

  check_for_valgrind_error
}

@test "${BATS_TEST_NUMBER}: \"${CMD} changedFile.docx\" displays a side-by-side comparison" {
  run ${DOCXBOX_BINARY} lorem "${PATH_DOCX}" "${PATH_NEW_DOCX}"

  local amount_chars_base=$(${DOCXBOX_BINARY} ls "${PATH_DOCX}" | wc --bytes)
  local amount_chars_diff=$(${DOCXBOX_BINARY} ls "${PATH_DOCX}" "${PATH_NEW_DOCX}" | wc --bytes)

  ${DOCXBOX_BINARY} ls "${PATH_DOCX}" "${PATH_NEW_DOCX}" | (( amount_chars_base < amount_chars_diff ))

  check_for_valgrind_error
}

@test "${BATS_TEST_NUMBER}: Output of ${CMD} nonexistent.docx\" is an error message" {
  run "$BATS_TEST_DIRNAME"/docxbox ls nonexistent.docx
  [ "$status" -ne 0 ]

  ${DOCXBOX_BINARY} ls nonexistent.docx 2>&1 | tee "${ERR_LOG}"
  cat "${ERR_LOG}" | grep --count "docxBox Error - File not found:"

  check_for_valgrind_error
}

@test "${BATS_TEST_NUMBER}: Output of ${CMD} wrong_file_type\" is an error message" {
  local wrong_file_types=(
  "test/tmp/cp_lorem_ipsum.pdf"
  "test/tmp/cp_mock_csv.csv"
  "test/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    ${DOCXBOX_BINARY} ls "${i}" 2>&1 | tee "${ERR_LOG}"
    check_for_valgrind_error
    cat "${ERR_LOG}" | grep --count "docxBox Error - File is no ZIP archive:"
  done
}
