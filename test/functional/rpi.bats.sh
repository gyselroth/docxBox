#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

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

PATH_EXTRACTED_IMAGE="test/tmp/unziped/word/media/image2.jpeg"
PATH_JPEG="test/assets/images/2100x400.jpeg"


BASE_COMMAND="docxbox rpi filename.docx"

@test "Case 1: Output of \"docxbox rpi {missing filename}\" is an error message" {
  run ${DOCXBOX_BINARY} rpi
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: DOCX filename" = "${lines[0]}" ]

  check_for_valgrind_error
}

@test "Case 2: Output of \"${BASE_COMMAND} {missing argument}\" is an error message" {
  run ${DOCXBOX_BINARY} rpi "${PATH_DOCX}"
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: Filename of image to be replaced" = "${lines[0]}" ]

  check_for_valgrind_error
}

missing_argument="imageName {missingReplacementImageName}"
@test "Case 3: Output of \"${BASE_COMMAND} ${missing_argument}\" is an error message" {
  run ${DOCXBOX_BINARY} rpi "${PATH_DOCX}" image2.jpeg
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: Filename of replacement image" = "${lines[0]}" ]

  check_for_valgrind_error
}

appendix="an image can be replaced"
@test "Case 4: With \"${BASE_COMMAND} imageName replacementImageName\" ${appendix}" {

  run ${DOCXBOX_BINARY} rpi "${PATH_DOCX}" image2.jpeg "${PATH_JPEG}"
  [ "$status" -eq 0 ]
  check_for_valgrind_error

  if [ ! -d test/tmp/unziped ]; then
    mkdir test/tmp/unziped;
    unzip "${PATH_DOCX}" -d test/tmp/unziped;
  fi

  file "${PATH_EXTRACTED_IMAGE}" | grep --count "2100x400"
}

arguments="imageName replacementImageName newFilename.docx"
appendix_new_docx="an image can be replaced and saved to new doxc"
@test "Case 5: With \"${BASE_COMMAND} ${arguments}\" ${appendix_new_docx}" {
  path_docx_out="test/tmp/newImage.docx"

  run ${DOCXBOX_BINARY} rpi "${PATH_DOCX}" image2.jpeg "${PATH_JPEG}" "${path_docx_out}"
  [ "$status" -eq 0 ]
  check_for_valgrind_error

  if [ ! -d test/tmp/unziped ]; then
    mkdir test/tmp/unziped;
    unzip test/tmp/newImage.docx -d test/tmp/unziped;
  fi

  file "${PATH_EXTRACTED_IMAGE}" | grep --count "2100x400"
}

@test "Case 6: Output of \"${BASE_COMMAND} nonexistent.image\" is an error message" {
  error_message="docxBox Error - Copy file failed - file not found:"

  ${DOCXBOX_BINARY} rpi "${PATH_DOCX}" image2.jpeg nonexistent.jpeg 2>&1 | tee "${ERR_LOG}"
  check_for_valgrind_error
  cat "${ERR_LOG}" | grep --count "${error_message}"
}

@test "Case 7: Output of \"docxbox rpi nonexistent.docx\" is an error message" {
  run ${DOCXBOX_BINARY} rpi nonexistent.docx
  [ "$status" -ne 0 ]

  ${DOCXBOX_BINARY} rpi nonexistent.docx 2>&1 image2.jpeg "${PATH_JPEG}" | tee "${ERR_LOG}"
  check_for_valgrind_error
  cat "${ERR_LOG}" | grep --count "docxBox Error - File not found:"
}

@test "Case 8: Output of \"docxbox rpi wrong_file_type\" is an error message" {
  pattern="docxBox Error - File is no ZIP archive:"
  wrong_file_types=(
  "test/tmp/cp_lorem_ipsum.pdf"
  "test/tmp/cp_mock_csv.csv"
  "test/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    ${DOCXBOX_BINARY} rpi "${i}" image2.jpeg "${PATH_JPEG}" 2>&1 | tee "${ERR_LOG}"
    check_for_valgrind_error
    cat "${ERR_LOG}" | grep --count "${pattern}"
  done
}
