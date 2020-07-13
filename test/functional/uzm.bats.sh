#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper
source ./test/functional/_set_docxbox_binary.sh

CMD="docxbox uzm"

PATH_DOCX="test/tmp/cp_table_unordered_list_images.docx"
ERR_LOG="test/tmp/err.log"

UNZIPPED_DOCX="cp_table_unordered_list_images.docx-media-extracted"

@test "${BATS_TEST_NUMBER}: \"${CMD} {missing argument}\" prints an error message" {
  local pattern="docxBox Error - Missing argument: Filename of DOCX to be extracted"

  run "${DOCXBOX_BINARY}" uzm
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"${CMD} nonexistent.docx\" prints an error message" {
  run "${DOCXBOX_BINARY}" uzm nonexistent.docx
  [ "$status" -ne 0 ]
  source ./test/functional/_check_for_valgrind_errors.sh

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
    source ./test/functional/_check_for_valgrind_errors.sh
    cat "${ERR_LOG}" | grep --count "${pattern}"
  done
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx\" extracts only media files" {
  run "${DOCXBOX_BINARY}" uzm "${PATH_DOCX}"

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: Unzipped files are located in project root" {
  ls | grep --count "${UNZIPPED_DOCX}"

  if [ -d "${UNZIPPED_DOCX}" ]; then
    rm --recursive "${UNZIPPED_DOCX}";
  fi
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx --media\" extracts only media files" {
  run "${DOCXBOX_BINARY}" uz "${PATH_DOCX}" --media

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: Unzipped files are located in project root after running uz --media " {
  ls | grep --count "${UNZIPPED_DOCX}"

  if [ -d "${UNZIPPED_DOCX}" ]; then
    rm --recursive "${UNZIPPED_DOCX}";
  fi
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx -m\" extracts only media files" {
  run "${DOCXBOX_BINARY}" uz "${PATH_DOCX}" -m

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: Unzipped files are located in project root after running uz -m" {
  ls | grep --count "${UNZIPPED_DOCX}"

  if [ -d "${UNZIPPED_DOCX}" ]; then
    rm --recursive "${UNZIPPED_DOCX}";
  fi
}
