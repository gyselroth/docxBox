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

@test "Case 1: \"docxbox v\" displays version number" {
  pattern="(^|\s)+(docxBox version )\K([0-9]|\.)*(?=\s|$)"

  ${DOCXBOX_BINARY} v | grep -Po "${pattern}"

  if $IS_VALGRIND_TEST; then
    n=$(cat "${VALGRIND_LOG}" | grep --count "${VALGRIND_ERR_PATTERN}" || /bin/true)
    if [ "$n" -eq 0 ]; then
      return 0
    else
      echo "There was a possible memory leak" >&2
      return 1
    fi
  fi
}
