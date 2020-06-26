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

path_docx="test/tmp/cp_table_unordered_list_images.docx"

base_command="docxbox lsmj filename.docx"

description="contains information about the creation time and date"

pattern="\"created\": \"2020-06-18T10:30:11Z\""

@test "Exit code of \"${base_command}\" is zero" {
  run ${DOCXBOX_BINARY} lsmj "${path_docx}"
  [ "$status" -eq 0 ]
}

@test "Output of \"docxbox lsmj {missing argument}\" is an error message" {
  pattern="docxBox Error - Missing argument: Filename of DOCX to be extracted"

  run ${DOCXBOX_BINARY} lsmj
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]
}

@test "Output of \"${base_command}\" contains information about the xml schema" {
  pattern="\"xmlSchema\": \"http://schemas.openxmlformats.org/officeDocument/2006\""

  ${DOCXBOX_BINARY} lsmj "${path_docx}" | grep --count "${pattern}"
}

@test "Output of \"docxbox lsm filename.docx --json\" ${description}" {
  ${DOCXBOX_BINARY} lsm "${path_docx}" --json | grep --count "${pattern}"
}

@test "Output of \"docxbox lsm filename.docx -j\" ${description}" {
  ${DOCXBOX_BINARY} lsm "${path_docx}" -j | grep --count "${pattern}"
}

@test "Output of \"docxbox ls filename.docx --meta --json\" ${description}" {
  ${DOCXBOX_BINARY} ls "${path_docx}" --meta --json | grep --count "${pattern}"
}

@test "Output of \"docxbox ls filename.docx -mj\" ${description}" {
  ${DOCXBOX_BINARY} ls "${path_docx}" -mj | grep --count "${pattern}"
}

@test "Output of \"${base_command}\" contains language information" {
  pattern="\"language\": \"en-US\""

  ${DOCXBOX_BINARY} lsmj "${path_docx}" | grep --count "${pattern}"
}

@test "Output of \"${base_command}\" contains information about the revision" {
  ${DOCXBOX_BINARY} lsmj "${path_docx}" | grep --count "\"revision\": \"2\""
}

@test "Output of \"docxbox lsmj nonexistent.docx\" is an error message" {
  err_log="test/tmp/err.log"

  run ${DOCXBOX_BINARY} lsmj nonexistent.docx
  [ "$status" -ne 0 ]

  ${DOCXBOX_BINARY} lsmj nonexistent.docx 2>&1 | tee "${err_log}"
  cat "${err_log}" | grep --count "docxBox Error - File not found:"
}

@test "Output of \"docxbox lsmj wrong_file_type\" is an error message" {
  pattern="docxBox Error - File is no ZIP archive:"
  err_log="test/tmp/err.log"
  wrong_file_types=(
  "test/tmp/cp_lorem_ipsum.pdf"
  "test/tmp/cp_mock_csv.csv"
  "test/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    ${DOCXBOX_BINARY} lsmj "${i}" 2>&1 | tee "${err_log}"
    cat "${err_log}" | grep --count "${pattern}"
  done
}
