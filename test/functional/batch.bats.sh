#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

VALGRIND_LOG="test/tmp/mem-leak.log"
VALGRIND="valgrind -v --leak-check=full\
 --log-file=${VALGRIND_LOG}"

VALGRIND_ERR_PATTERN="ERROR SUMMARY: [0-9] errors from [0-9] contexts"

if $IS_VALGRIND_TEST;
then
  DOCXBOX_BINARY="${VALGRIND} $BATS_TEST_DIRNAME/../tmp/docxbox"
else
  DOCXBOX_BINARY="$BATS_TEST_DIRNAME/../tmp/docxbox"
fi

PATH_DOCX="test/tmp/cp_table_unordered_list_images.docx"
PATH_DOCX_PLAINTEXT="test/tmp/cp_plain_text.docx"
BASE_COMMAND="docxbox batch filename.docx"

title_one_missing_argument="Case 1: Output of \"docxbox batch \
{missing argument}\" is an error message"
@test "${title_one_missing_argument}" {
  run ${DOCXBOX_BINARY} batch
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: DOCX file" = "${lines[0]}" ]

  check_for_valgrind_error
}

title_two_missing_arguments="Case 2:Output of \"${BASE_COMMAND} \
{missing argument}\" is an error message"
@test "${title_two_missing_arguments}" {
  run ${DOCXBOX_BINARY} batch "${PATH_DOCX}"
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: Batch commands JSON" = "${lines[0]}" ]

  check_for_valgrind_error
}

# TODO(Lucas): Add testcase batch {1:mm,2:rpt}
# TODO(Lucas): Add testcase batch {1:mm,2:rmt}

APPENDIX="a batch sequence can be executed"
title_mm="Case 3: With \"${BASE_COMMAND} batch_sequence_as_JSON\" ${APPENDIX} \
and the meta attribute title gets changed"
@test "${title_mm}" {
  batch="{\"1\":{\"mm\":[\"title\",\"foo\"]}}"
  run ${DOCXBOX_BINARY} batch "${PATH_DOCX}" "${batch}"

  check_for_valgrind_error

  ${DOCXBOX_BINARY} lsm ${PATH_DOCX} | grep --count "title: foo"
}

title_rpt="Case 4: With \"${BASE_COMMAND} batch_sequence_as_JSON\" ${APPENDIX} \
and a string can be replaced with the \"rpt\" command"
@test "${title_rpt}" {
  batch="{\"1\":{\"rpt\":[\"text\",\"FooBar\"]}}"

  bytes_before_batch=$(${DOCXBOX_BINARY} txt "${PATH_DOCX_PLAINTEXT}" | wc --bytes)

  run ${DOCXBOX_BINARY} batch "${PATH_DOCX_PLAINTEXT}" ${batch}

  check_for_valgrind_error

  bytes_after_batch=$(${DOCXBOX_BINARY} txt ${PATH_DOCX} | wc --bytes)

  (( bytes_before_batch > bytes_after_batch ))
}

title_rmt="Case 5: With \"${BASE_COMMAND} batch_sequence_as_JSON\" ${APPENDIX} \
and a string can be replaced with the \"rmt\" command"
@test "${title_rmt}" {
  batch="{\"1\":{\"rmt\":[\"THIS\",\"TITLE\"]}}"

  wc_before_batch=$(${DOCXBOX_BINARY} txt "${PATH_DOCX_PLAINTEXT}" | wc --words)

  run ${DOCXBOX_BINARY} batch "${PATH_DOCX_PLAINTEXT}" ${batch}

  check_for_valgrind_error

  wc_after_batch=$(${DOCXBOX_BINARY} txt "${PATH_DOCX_PLAINTEXT}" | wc --words)

  (( wc_before_batch > wc_after_batch ))
}

check_for_valgrind_error() {
  if $IS_VALGRIND_TEST; then
    cat "${VALGRIND_LOG}" | grep --count --invert-match "${VALGRIND_ERR_PATTERN}"
  fi
}
