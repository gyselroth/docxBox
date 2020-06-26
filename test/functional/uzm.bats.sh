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

description="only media files are extracted"
unzipped_docx="cp_table_unordered_list_images.docx-media-extracted"

@test "Output of \"docxbox uzm {missing argument}\" is an error message" {
  pattern="docxBox Error - Missing argument: Filename of DOCX to be extracted"

  run ${DOCXBOX_BINARY} uzm
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]
}

@test "Output of \"docxbox uzm nonexistent.docx\" is an error message" {
  err_log="test/tmp/err.log"

  run ${DOCXBOX_BINARY} uzm nonexistent.docx
  [ "$status" -ne 0 ]

  ${DOCXBOX_BINARY} uzm nonexistent.docx 2>&1 | tee "${err_log}"
  cat "${err_log}" | grep --count "docxBox Error - File not found:"
}

@test "Output of \"docxbox uzm wrong_file_type\" is an error message" {
  pattern="docxBox Error - File is no ZIP archive:"
  err_log="test/tmp/err.log"
  wrong_file_types=(
  "test/tmp/cp_lorem_ipsum.pdf"
  "test/tmp/cp_mock_csv.csv"
  "test/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    ${DOCXBOX_BINARY} uzm "${i}" 2>&1 | tee "${err_log}"
    cat "${err_log}" | grep --count "${pattern}"
  done
}

@test "With \"docxbox uzm filename.docx\" ${description}" {
  run ${DOCXBOX_BINARY} uzm "${path_docx}"
}

@test "Unzipped files are located in project root" {
  ls | grep --count "${unzipped_docx}"

  if [ -d "${unzipped_docx}" ]; then
    rm --recursive "${unzipped_docx}";
  fi
}

@test "With \"docxbox uz filename.docx --media\" ${description}" {
  run ${DOCXBOX_BINARY} uz "${path_docx}" --media
}

@test "Unzipped files are located in project root after running uz --media " {
  ls | grep --count "${unzipped_docx}"

  if [ -d "${unzipped_docx}" ]; then
    rm --recursive "${unzipped_docx}";
  fi
}

@test "With \"docxbox uz filename.docx -m\" ${description}" {
  run ${DOCXBOX_BINARY} uz "${path_docx}" -m
}

@test "Unzipped files are located in project root after running uz -m" {
  ls | grep --count "${unzipped_docx}"

  if [ -d "${unzipped_docx}" ]; then
    rm --recursive "${unzipped_docx}";
  fi
}
