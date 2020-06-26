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

base_command="docxbox lsdj filename.docx"

path_docx="test/tmp/cp_table_unordered_list_images.docx"
long_description_json="the fields in the docx are listed as JSON"

@test "Exit code of \"${base_command}\" is zero" {
  run ${DOCXBOX_BINARY} lsdj "${path_docx}"
  [ "$status" -eq 0 ]
}

@test "Output of \"docxbox lsdj {missing argument}\" is an error message" {
  run ${DOCXBOX_BINARY} lsdj
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: Filename of DOCX to be extracted" = "${lines[0]}" ]
}

@test "With \"${base_command}\" the fields in the docx are listed as JSON" {
  pattern="table_unordered_list_images.docx-"
  ${DOCXBOX_BINARY} lsdj "${path_docx}" | grep --count "${pattern}"

  pattern="/word/document.xml"
  ${DOCXBOX_BINARY} lsdj "${path_docx}" | grep --count "${pattern}"
}

longhand="--fields --json"
title="With \"docxbox ls filename.docx ${longhand}\" "
title+="${long_description_json}"
@test "${title}" {
  pattern="table_unordered_list_images.docx-"
  ${DOCXBOX_BINARY} ls "${path_docx}" ${longhand} | grep --count "${pattern}"

  pattern="/word/document.xml"
  ${DOCXBOX_BINARY} ls "${path_docx}" ${longhand} | grep --count "${pattern}"
}

@test "With \"docxbox ls filename.docx -dj\" ${long_description_json}" {
  pattern="table_unordered_list_images.docx-"
  ${DOCXBOX_BINARY} ls "${path_docx}" -dj | grep --count "${pattern}"

  pattern="/word/document.xml"
  ${DOCXBOX_BINARY} ls "${path_docx}" -dj | grep --count "${pattern}"
}

@test "With \"docxbox lsd filename.docx --json\" ${long_description_json}" {
  pattern="table_unordered_list_images.docx-"
  ${DOCXBOX_BINARY} lsd "${path_docx}" --json | grep --count "${pattern}"

  pattern="/word/document.xml"
  ${DOCXBOX_BINARY} lsd "${path_docx}" --json | grep --count "${pattern}"
}

@test "Output of \"docxbox lsdj nonexistent.docx\" is an error message" {
  err_log="test/tmp/err.log"

  run ${DOCXBOX_BINARY} lsdj nonexistent.docx
  [ "$status" -ne 0 ]

  ${DOCXBOX_BINARY} lsdj nonexistent.docx 2>&1 | tee "${err_log}"
  cat "${err_log}" | grep --count "docxBox Error - File not found:"
}

@test "Output of \"${base_command} wrong_file_type\" is an error message" {
  err_log="test/tmp/err.log"
  wrong_file_types=(
  "test/tmp/cp_lorem_ipsum.pdf"
  "test/tmp/cp_mock_csv.csv"
  "test/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    ${DOCXBOX_BINARY} lsdj "${i}" 2>&1 | tee "${err_log}"
    cat "${err_log}" | grep --count "docxBox Error - File is no ZIP archive:"
  done
}

