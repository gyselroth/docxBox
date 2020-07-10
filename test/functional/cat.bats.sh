#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper
source ./test/functional/_set_docxbox_binary.sh

CMD="docxbox cat"

PATH_DOCX="test/tmp/cp_table_unordered_list_images.docx"
PATH_XML="word/document.xml"

@test "${BATS_TEST_NUMBER}: \"${CMD} {missing argument}\" prints an error message" {
  run ${DOCXBOX_BINARY} cat
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: DOCX filename" = "${lines[0]}" ]

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx {missing argument}\" prints an error message" {
  run ${DOCXBOX_BINARY} cat "${PATH_DOCX}"
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: File to be displayed" = "${lines[0]}" ]

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx filename.xml\" displays the XML " {
  ${DOCXBOX_BINARY} cat "${PATH_DOCX}" "${PATH_XML}" | grep "^[[:space:]]\{4\}"

  source ./test/functional/_check_for_valgrind_errors.sh
}
