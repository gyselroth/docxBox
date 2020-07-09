#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

CMD="docxbox uzm"

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

UNZIPPED_DOCX="cp_table_unordered_list_images.docx-media-extracted"

@test "${BATS_TEST_NUMBER}: \"${CMD} {missing argument}\" prints an error message" {
  local pattern="docxBox Error - Missing argument: Filename of DOCX to be extracted"

  run ${DOCXBOX_BINARY} uzm
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]

  check_for_valgrind_error
}

@test "${BATS_TEST_NUMBER}: \"${CMD} nonexistent.docx\" prints an error message" {
  run ${DOCXBOX_BINARY} uzm nonexistent.docx
  [ "$status" -ne 0 ]
  check_for_valgrind_error

  ${DOCXBOX_BINARY} uzm nonexistent.docx 2>&1 | tee "${ERR_LOG}"
  cat "${ERR_LOG}" | grep --count "docxBox Error - File not found:"
}

@test "${BATS_TEST_NUMBER}: \"${CMD} wrong_file_type\" prints an error message" {
  local pattern="docxBox Error - File is no ZIP archive:"
  local wrong_file_types=(
  "test/tmp/cp_lorem_ipsum.pdf"
  "test/tmp/cp_mock_csv.csv"
  "test/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    ${DOCXBOX_BINARY} uzm "${i}" 2>&1 | tee "${ERR_LOG}"
    check_for_valgrind_error
    cat "${ERR_LOG}" | grep --count "${pattern}"
  done
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx\" extracts only media files" {
  run ${DOCXBOX_BINARY} uzm "${PATH_DOCX}"

  check_for_valgrind_error
}

@test "${BATS_TEST_NUMBER}: Unzipped files are located in project root" {
  ls | grep --count "${UNZIPPED_DOCX}"

  if [ -d "${UNZIPPED_DOCX}" ]; then
    rm --recursive "${UNZIPPED_DOCX}";
  fi
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx --media\" extracts only media files" {
  run ${DOCXBOX_BINARY} uz "${PATH_DOCX}" --media

  check_for_valgrind_error
}

@test "${BATS_TEST_NUMBER}: Unzipped files are located in project root after running uz --media " {
  ls | grep --count "${UNZIPPED_DOCX}"

  if [ -d "${UNZIPPED_DOCX}" ]; then
    rm --recursive "${UNZIPPED_DOCX}";
  fi
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx -m\" extracts only media files" {
  run ${DOCXBOX_BINARY} uz "${PATH_DOCX}" -m

  check_for_valgrind_error
}

@test "${BATS_TEST_NUMBER}: Unzipped files are located in project root after running uz -m" {
  ls | grep --count "${UNZIPPED_DOCX}"

  if [ -d "${UNZIPPED_DOCX}" ]; then
    rm --recursive "${UNZIPPED_DOCX}";
  fi
}
