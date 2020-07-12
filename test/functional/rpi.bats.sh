#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper
source ./test/functional/_set_docxbox_binary.sh

CMD="docxbox rpi"

PATH_DOCX="test/tmp/cp_table_unordered_list_images.docx"
ERR_LOG="test/tmp/err.log"

PATH_EXTRACTED_IMAGE="test/tmp/unziped/word/media/image2.jpeg"
PATH_JPEG="test/assets/images/2100x400.jpeg"

@test "${BATS_TEST_NUMBER}: \"${CMD} {missing filename}\" prints an error message" {
  run "${DOCXBOX_BINARY}" rpi
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: DOCX filename" = "${lines[0]}" ]

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx {missing argument}\" prints an error message" {
  run "${DOCXBOX_BINARY}" rpi "${PATH_DOCX}"
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: Filename of image to be replaced" = "${lines[0]}" ]

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx imageName {missingReplacementImageName}\" prints an error message" {
  run "${DOCXBOX_BINARY}" rpi "${PATH_DOCX}" image2.jpeg
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: Filename of replacement image" = "${lines[0]}" ]

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx imageName replacementImageName\" replaces an image" {

  run "${DOCXBOX_BINARY}" rpi "${PATH_DOCX}" image2.jpeg "${PATH_JPEG}"
  [ "$status" -eq 0 ]
  source ./test/functional/_check_for_valgrind_errors.sh

  if [ ! -d test/tmp/unziped ]; then
    mkdir test/tmp/unziped;
    unzip "${PATH_DOCX}" -d test/tmp/unziped;
  fi

  file "${PATH_EXTRACTED_IMAGE}" | grep --count "2100x400"
}

@test "${BATS_TEST_NUMBER}: \"${CMD} imageName replacementImageName newFilename.docx\" replaces image and saves to new docx" {
  local path_docx_out="test/tmp/newImage.docx"

  run "${DOCXBOX_BINARY}" rpi "${PATH_DOCX}" image2.jpeg "${PATH_JPEG}" "${path_docx_out}"
  [ "$status" -eq 0 ]
  source ./test/functional/_check_for_valgrind_errors.sh

  if [ ! -d test/tmp/unziped ]; then
    mkdir test/tmp/unziped;
    unzip test/tmp/newImage.docx -d test/tmp/unziped;
  fi

  file "${PATH_EXTRACTED_IMAGE}" | grep --count "2100x400"
}

@test "${BATS_TEST_NUMBER}: \"${CMD} nonexistent.image\" prints an error message" {
  local error_message="docxBox Error - Copy file failed - file not found:"

  ${DOCXBOX_BINARY} rpi "${PATH_DOCX}" image2.jpeg nonexistent.jpeg 2>&1 | tee "${ERR_LOG}"
  source ./test/functional/_check_for_valgrind_errors.sh
  cat "${ERR_LOG}" | grep --count "${error_message}"
}

@test "${BATS_TEST_NUMBER}: \"${CMD} nonexistent.docx\" prints an error message" {
  run "${DOCXBOX_BINARY}" rpi nonexistent.docx
  [ "$status" -ne 0 ]

  ${DOCXBOX_BINARY} rpi nonexistent.docx 2>&1 image2.jpeg "${PATH_JPEG}" | tee "${ERR_LOG}"
  source ./test/functional/_check_for_valgrind_errors.sh
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
    ${DOCXBOX_BINARY} rpi "${i}" image2.jpeg "${PATH_JPEG}" 2>&1 | tee "${ERR_LOG}"
    source ./test/functional/_check_for_valgrind_errors.sh
    cat "${ERR_LOG}" | grep --count "${pattern}"
  done
}
