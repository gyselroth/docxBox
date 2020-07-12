#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper
source ./test/functional/_set_docxbox_binary.sh

CMD="docxbox batch"

PATH_DOCX="test/tmp/cp_table_unordered_list_images.docx"
PATH_DOCX_PLAINTEXT="test/tmp/cp_plain_text.docx"
PATH_DOCX_MERGEFIELD="test/tmp/cp_mergefields.docx"


@test "$BATS_TEST_NUMBER: \"${CMD} {missing argument}\" prints an error message" {
  run ${DOCXBOX_BINARY} batch
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: DOCX file" = "${lines[0]}" ]

  source ./test/functional/_check_for_valgrind_errors.sh
}


@test "$BATS_TEST_NUMBER: \"${CMD} filename.docx {missing argument}\" prints an error message" {
  run ${DOCXBOX_BINARY} batch "${PATH_DOCX}"
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: Batch commands JSON" = "${lines[0]}" ]

  source ./test/functional/_check_for_valgrind_errors.sh
}

# TODO(Lucas): Add testcase batch {1:mm,2:rpt}
# TODO(Lucas): Add testcase batch {1:mm,2:rmt}

@test "$BATS_TEST_NUMBER: \"${CMD} batch_sequence_as_JSON\" executes a batch sequence, meta attribute title gets changed" {
  local batch="{\"1\":{\"mm\":[\"title\",\"foo\"]}}"
  run ${DOCXBOX_BINARY} batch "${PATH_DOCX}" "${batch}"

  source ./test/functional/_check_for_valgrind_errors.sh

  ${DOCXBOX_BINARY} lsm ${PATH_DOCX} | grep --count "title: foo"
}

@test "$BATS_TEST_NUMBER: \"${CMD} batch_sequence_as_JSON\" executes a batch sequence, string gets replaced with the \"rpt\" command" {
  local batch="{\"1\":{\"rpt\":[\"text\",\"FooBar\"]}}"

  local bytes_before_batch
  bytes_before_batch=$(${DOCXBOX_BINARY} txt "${PATH_DOCX_PLAINTEXT}" | wc --bytes)

  run ${DOCXBOX_BINARY} batch "${PATH_DOCX_PLAINTEXT}" "${batch}"

  source ./test/functional/_check_for_valgrind_errors.sh

  ${DOCXBOX_BINARY} txt ${PATH_DOCX_PLAINTEXT} | grep --count "FooBar"

  local bytes_after_batch
  bytes_after_batch=$(${DOCXBOX_BINARY} txt ${PATH_DOCX_PLAINTEXT} | wc --bytes)

  (( bytes_before_batch < bytes_after_batch ))
}

@test "$BATS_TEST_NUMBER: \"${CMD} batch_sequence_as_JSON\" executes a batch sequence, string gets replaced with the \"rmt\" command" {
  local batch="{\"1\":{\"rmt\":[\"THIS\",\"TITLE\"]}}"

  local wc_before_batch
  wc_before_batch=$(${DOCXBOX_BINARY} txt "${PATH_DOCX_PLAINTEXT}" | wc --words)

  run ${DOCXBOX_BINARY} batch "${PATH_DOCX_PLAINTEXT}" "${batch}"

  source ./test/functional/_check_for_valgrind_errors.sh

  local wc_after_batch
  wc_after_batch=$(${DOCXBOX_BINARY} txt "${PATH_DOCX_PLAINTEXT}" | wc --words)

  (( wc_before_batch > wc_after_batch ))
}

@test "$BATS_TEST_NUMBER: \"${CMD} batch_sequence_as_JSON\" executes a batch sequence, mergefields get replaced with the \"sfv\" command" {
  local batch="{\"1\":{\"sfv\":[\"MERGEFIELD  Mergefield_One\",\"FooBar\"]}}"

  run ${DOCXBOX_BINARY} batch "${PATH_DOCX_MERGEFIELD}" "${batch}"

  source ./test/functional/_check_for_valgrind_errors.sh

  ${DOCXBOX_BINARY} txt "${PATH_DOCX_MERGEFIELD}" | grep --count "FooBar"
}
