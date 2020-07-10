#!/usr/bin/env bash

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

source ./test/functional/_set_valgrind_variables.sh

if $IS_VALGRIND_TEST; then
  n=$(cat "${VALGRIND_LOG}" | grep --count "${VALGRIND_ERR_PATTERN}" || /bin/true)
  if [ "$n" -eq 0 ]; then
    return 0
  else
    echo "There was a possible memory leak" >&2
    return 1
  fi
fi
