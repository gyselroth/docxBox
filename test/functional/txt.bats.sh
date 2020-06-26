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

base_command="docxbox txt filename.docx"

@test "Output of \"docxbox txt {missing argument}\" is an error message" {
  run ${DOCXBOX_BINARY} txt
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: Filename of DOCX to be extracted" = "${lines[0]}" ]
}

@test "Output of \"${base_command}\" is the the plain text from given file" {
  ${DOCXBOX_BINARY} txt "${path_docx}" | grep --count "Officia"
}

appendix="is the segmented plain text from given file"
@test "Output of \"${base_command} -s \" ${appendix}" {
  ${DOCXBOX_BINARY} txt "${path_docx}" -s | wc --lines | grep --count "582"
  ${DOCXBOX_BINARY} txt "${path_docx}" | wc --lines | grep --count "27"
}

@test "Output of \"${base_command} --segments \" ${appendix}" {
  ${DOCXBOX_BINARY} txt "${path_docx}" --segments | wc --lines | grep --count "582"
  ${DOCXBOX_BINARY} txt "${path_docx}" | wc --lines | grep --count "27"
}

@test "Output of \"docxbox txt nonexistent.docx\" is an error message" {
  err_log="test/tmp/err.log"

  run ${DOCXBOX_BINARY} txt nonexistent.docx
  [ "$status" -ne 0 ]

  ${DOCXBOX_BINARY} txt nonexistent.docx 2>&1 | tee "${err_log}"
  cat "${err_log}" | grep --count "docxBox Error - File not found:"
}

@test "Output of \"docxbox txt wrong_file_type\" is an error message" {
  pattern="docxBox Error - File is no ZIP archive:"
  err_log="test/tmp/err.log"
  wrong_file_types=(
  "test/tmp/cp_lorem_ipsum.pdf"
  "test/tmp/cp_mock_csv.csv"
  "test/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    ${DOCXBOX_BINARY} txt "${i}" 2>&1 | tee "${err_log}"
    cat "${err_log}" | grep --count "${pattern}"
  done
}
