#!/usr/bin/env bash

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

VALGRIND_LOG="test/tmp/mem-leak.log"

# shellcheck disable=SC2034
VALGRIND="valgrind -v --leak-check=full\
 --log-file=${VALGRIND_LOG}"

# shellcheck disable=SC2034
VALGRIND_ERR_PATTERN="ERROR SUMMARY: [1-9] errors from [1-9] contexts"
