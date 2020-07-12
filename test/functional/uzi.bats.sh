#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper
source ./test/functional/_set_docxbox_binary.sh

CMD="docxbox uzi"

PATH_DOCX="test/tmp/cp_bio_assay.docx"
ERR_LOG="test/tmp/err.log"

UNZIPPED_FOLDER="cp_bio_assay.docx-extracted"

@test "${BATS_TEST_NUMBER}: \"${CMD} {missing argument}\" prints an error message" {
  local pattern="docxBox Error - Missing argument: Filename of DOCX to be extracted"

  run "${DOCXBOX_BINARY}" uzi
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"${CMD} nonexistent.docx\" prints an error message" {
  run "${DOCXBOX_BINARY}" uzi nonexistent.docx
  [ "$status" -ne 0 ]
  source ./test/functional/_check_for_valgrind_errors.sh

  ${DOCXBOX_BINARY} uzi nonexistent.docx 2>&1 | tee "${ERR_LOG}"
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
    ${DOCXBOX_BINARY} uzi "${i}" 2>&1 | tee "${ERR_LOG}"
    source ./test/functional/_check_for_valgrind_errors.sh
    cat "${ERR_LOG}" | grep --count "${pattern}"
  done
}

@test "${BATS_TEST_NUMBER}: \"docxbox uzi filename.docx\" unzippes all files" {
  run "${DOCXBOX_BINARY}" uzi "${PATH_DOCX}"
  [ "$status" -eq 0 ]
  source ./test/functional/_check_for_valgrind_errors.sh

  cat "${UNZIPPED_FOLDER}/word/document.xml" | grep "^[[:space:]]\{4\}"
}

@test "${BATS_TEST_NUMBER}: Unzipped files are located in project root" {
  ls | grep --count "${UNZIPPED_FOLDER}"

  if [ -d "${UNZIPPED_FOLDER}" ]; then
    rm --recursive "${UNZIPPED_FOLDER}";
  fi
}

@test "${BATS_TEST_NUMBER}: \"docxbox uz filename.docx -i\" unzippes all files" {
  run "${DOCXBOX_BINARY}" uz "${PATH_DOCX}" -i
  [ "$status" -eq 0 ]

  source ./test/functional/_check_for_valgrind_errors.sh

  cat "${UNZIPPED_FOLDER}/word/document.xml" | grep "^[[:space:]]\{4\}"
}

@test "${BATS_TEST_NUMBER}: Unzipped files are located in project root after running uz -i" {
  ls | grep --count "${UNZIPPED_FOLDER}"

  if [ -d "${UNZIPPED_FOLDER}" ]; then
    rm --recursive "${UNZIPPED_FOLDER}";
  fi
}

@test "${BATS_TEST_NUMBER}: \"docxbox uz filename.docx --indent\" unzippes all files" {
  run "${DOCXBOX_BINARY}" uz "${PATH_DOCX}" --indent
  [ "$status" -eq 0 ]

  source ./test/functional/_check_for_valgrind_errors.sh

  cat "${UNZIPPED_FOLDER}/word/document.xml" | grep "^[[:space:]]\{4\}"
}

@test "${BATS_TEST_NUMBER}: Unzipped files are located in project root after running uz --indent" {
  ls | grep --count "${UNZIPPED_FOLDER}"

  if [ -d "${UNZIPPED_FOLDER}" ]; then
    rm --recursive "${UNZIPPED_FOLDER}";
  fi
}
