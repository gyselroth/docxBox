#!/usr/bin/env bash

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

case "$OSTYPE" in
  bsd*)     OS="bsd" ;;
  darwin*)  OS="osx" ;;
  linux*)   OS="linux" ;;
  solaris*) OS="solaris" ;;
  *)        OS="unknown: $OSTYPE" ;;
esac

setup() {
  if [ ! -d test/tmp ]; then
    mkdir "test/tmp"
  fi

  if [ "$OS" == "osx" ]; then
    cp "$BATS_TEST_DIRNAME"/../../bin/mac/docxbox "$BATS_TEST_DIRNAME"/../tmp/docxbox
  elif [ "$OS" == "linux" ]; then
    cp "$BATS_TEST_DIRNAME"/../../bin/linux/docxbox "$BATS_TEST_DIRNAME"/../tmp/docxbox
  fi

  bash ./test/functional/_copy_mockfiles.sh
}

teardown() {
  bash ./test/functional/_delete_copied_mockfiles.sh
}

check_for_valgrind_error() {
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
