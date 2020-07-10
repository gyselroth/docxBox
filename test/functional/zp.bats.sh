#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper
source ./test/functional/_set_docxbox_binary.sh

CMD="docxbox zp"

UNZIPPED_DOCX_DIRECTORY="test/tmp/unzipped"

@test "${BATS_TEST_NUMBER}: \"${CMD} {missing argument}\" prints an error message" {
  local pattern="docxBox Error - Missing argument: Path of directory to be zipped"

  run ${DOCXBOX_BINARY} zp
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"${CMD} directory {missing argument}\" prints an error message" {
  local pattern="docxBox Error - Missing argument: Filename of docx to be created"

  run ${DOCXBOX_BINARY} zp "${UNZIPPED_DOCX_DIRECTORY}"
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"${CMD} directory /path-to-file/filename.docx\" zips directory into docx" {
  if [ ! -d "${UNZIPPED_DOCX_DIRECTORY}" ]; then
    mkdir "${UNZIPPED_DOCX_DIRECTORY}";
    path_docx="test/tmp/cp_table_unordered_list_images.docx"
    unzip $path_docx -d "${UNZIPPED_DOCX_DIRECTORY}";
  fi

  path_new_docx="test/tmp/zp_table_unordered_list_images.docx"

  run ${DOCXBOX_BINARY} zp "${UNZIPPED_DOCX_DIRECTORY}" "${path_new_docx}"
  [ "$status" -eq 0 ]

  source ./test/functional/_check_for_valgrind_errors.sh

  ls test/tmp | grep -c zp_table_unordered_list_images.docx
}
