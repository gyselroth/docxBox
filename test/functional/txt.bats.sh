#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper
source ./test/functional/_set_docxbox_binary.sh

CMD="docxbox txt"

PATH_DOCX="test/tmp/cp_table_unordered_list_images.docx"
ERR_LOG="test/tmp/err.log"

@test "${BATS_TEST_NUMBER}: \"${CMD} {missing argument}\" prints an error message" {
  run ${DOCXBOX_BINARY} txt
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: Filename of DOCX to be extracted" = "${lines[0]}" ]

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx\" displays plain text of given file" {
  ${DOCXBOX_BINARY} txt "${PATH_DOCX}" | grep --count "Officia"

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx -s\" displays segmented plain text of given file" {
  local segmented=$(${DOCXBOX_BINARY} txt "${PATH_DOCX}" -s | wc --lines)
  local non_segmented=$(${DOCXBOX_BINARY} txt "${PATH_DOCX}" | wc --lines)

  source ./test/functional/_check_for_valgrind_errors.sh

  (( segmented > non_segmented ))

}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx --segments\" displays segmented plain text of given file" {
  local segmented=$(${DOCXBOX_BINARY} txt "${PATH_DOCX}" --segments | wc --lines)
  local non_segmented=$(${DOCXBOX_BINARY} txt "${PATH_DOCX}" | wc --lines)

  source ./test/functional/_check_for_valgrind_errors.sh

  (( segmented > non_segmented ))
}

@test "${BATS_TEST_NUMBER}: \"${CMD} nonexistent.docx\" prints an error message" {
  run ${DOCXBOX_BINARY} txt nonexistent.docx
  [ "$status" -ne 0 ]

  source ./test/functional/_check_for_valgrind_errors.sh

  ${DOCXBOX_BINARY} txt nonexistent.docx 2>&1 | tee "${ERR_LOG}"
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
    ${DOCXBOX_BINARY} txt "${i}" 2>&1 | tee "${ERR_LOG}"
    source ./test/functional/_check_for_valgrind_errors.sh
    cat "${ERR_LOG}" | grep --count "${pattern}"
  done
}
