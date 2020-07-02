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

DESCRIPTION="only media files are extracted"
UNZIPPED_DOCX="cp_table_unordered_list_images.docx-media-extracted"

@test "Output of \"docxbox uzm {missing argument}\" is an error message" {
  pattern="docxBox Error - Missing argument: Filename of DOCX to be extracted"

  run ${DOCXBOX_BINARY} uzm
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]
}

@test "Output of \"docxbox uzm nonexistent.docx\" is an error message" {
  run ${DOCXBOX_BINARY} uzm nonexistent.docx
  [ "$status" -ne 0 ]

  ${DOCXBOX_BINARY} uzm nonexistent.docx 2>&1 | tee "${ERR_LOG}"
  cat "${ERR_LOG}" | grep --count "docxBox Error - File not found:"
}

@test "Output of \"docxbox uzm wrong_file_type\" is an error message" {
  pattern="docxBox Error - File is no ZIP archive:"
  wrong_file_types=(
  "test/tmp/cp_lorem_ipsum.pdf"
  "test/tmp/cp_mock_csv.csv"
  "test/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    ${DOCXBOX_BINARY} uzm "${i}" 2>&1 | tee "${ERR_LOG}"
    cat "${ERR_LOG}" | grep --count "${pattern}"
  done
}

@test "With \"docxbox uzm filename.docx\" ${DESCRIPTION}" {
  run ${DOCXBOX_BINARY} uzm "${PATH_DOCX}"
}

@test "Unzipped files are located in project root" {
  ls | grep --count "${UNZIPPED_DOCX}"

  if [ -d "${UNZIPPED_DOCX}" ]; then
    rm --recursive "${UNZIPPED_DOCX}";
  fi
}

@test "With \"docxbox uz filename.docx --media\" ${DESCRIPTION}" {
  run ${DOCXBOX_BINARY} uz "${PATH_DOCX}" --media
}

@test "Unzipped files are located in project root after running uz --media " {
  ls | grep --count "${UNZIPPED_DOCX}"

  if [ -d "${UNZIPPED_DOCX}" ]; then
    rm --recursive "${UNZIPPED_DOCX}";
  fi
}

@test "With \"docxbox uz filename.docx -m\" ${DESCRIPTION}" {
  run ${DOCXBOX_BINARY} uz "${PATH_DOCX}" -m
}

@test "Unzipped files are located in project root after running uz -m" {
  ls | grep --count "${UNZIPPED_DOCX}"

  if [ -d "${UNZIPPED_DOCX}" ]; then
    rm --recursive "${UNZIPPED_DOCX}";
  fi
}
