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

LONGHAND_COMMAND="docxbox ls filename.docx"
DESCRIPTION="contains files' and directories' attributes"

ATTRIBUTES=(
  "length"
  "date"
  "time"
  "file")

@test "Output of \"docxbox lsj filename.docx\" ${DESCRIPTION}" {
  for i in "${ATTRIBUTES[@]}"
  do
    ${DOCXBOX_BINARY} lsj "${PATH_DOCX}" | grep --count "${i}"
  done
}

@test "Output of \"${LONGHAND_COMMAND} --json\" ${DESCRIPTION}" {
  for i in "${ATTRIBUTES[@]}"
  do
    ${DOCXBOX_BINARY} ls "${PATH_DOCX}" --json | grep --count "${i}"
  done
}

@test "Output of \"${LONGHAND_COMMAND} -j\" ${DESCRIPTION}" {
  for i in "${ATTRIBUTES[@]}"
  do
    ${DOCXBOX_BINARY} ls "${PATH_DOCX}" -j | grep --count "${i}"
  done
}

@test "Output of \"docxbox lsj filename.docx\" are contained files" {
search_values=(
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
    ${DOCXBOX_BINARY} lsj "${PATH_DOCX}" | grep --count "${i}"
  done
}

@test "Output of \"docxbox lsj filename.docx\" contains files' date and time" {
  ${DOCXBOX_BINARY} lsj "${PATH_DOCX}" | grep --count "7/./2020"
  ${DOCXBOX_BINARY} lsj "${PATH_DOCX}" | grep --count "7/3/2020"
}

@test "Output of \"docxbox lsj {missing argument}\" is an error message" {
  run ${DOCXBOX_BINARY} lsj
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: DOCX filename" = "${lines[0]}" ]
}

@test "Output of \"docxbox lsj nonexistent.docx\" is an error message" {
  run ${DOCXBOX_BINARY} lsj nonexistent.docx
  [ "$status" -ne 0 ]

  ${DOCXBOX_BINARY} lsj nonexistent.docx 2>&1 | tee "${ERR_LOG}"
  cat "${ERR_LOG}" | grep --count "docxBox Error - File not found:"
}

@test "Output of \"docxbox lsj wrong_file_type\" is an error message" {
  wrong_file_types=(
  "test/tmp/cp_lorem_ipsum.pdf"
  "test/tmp/cp_mock_csv.csv"
  "test/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    ${DOCXBOX_BINARY} lsj "${i}" 2>&1 | tee "${ERR_LOG}"
    cat "${ERR_LOG}" | grep --count "docxBox Error - File is no ZIP archive:"
  done
}
