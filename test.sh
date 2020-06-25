#!/bin/bash

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

################################################################################
# Run functional tests written with bats - https://github.com/sstephenson/bats #
#                                                                              #
# Install bats on Linux: sudo apt-get install bats                             #
# Install bats on Mac: brew install bats                                       #
################################################################################

IS_ERROR=false
AMOUNT_ERRORS=0

PATH_TESTS_FUNCTIONAL="./test/functional"
PATH_TESTS_VALGRIND="./test/valgrind"
IS_VALGRIND_TEST=false

export DOCXBOX_BINARY

function run_single_case() {
  START_TIME=$SECONDS

  bats ${PATH_TESTS_FUNCTIONAL}/"$1".bats.sh

  if [ $? -ne 0 ]; then
    IS_ERROR=true
    ((AMOUNT_ERRORS=AMOUNT_ERRORS+1))
  fi
}

function run_all_cases() {
  export IS_VALGRIND_TEST=$IS_VALGRIND_TEST

  START_TIME=$SECONDS

  # Meta commands
  printf "\033[4mTest display of help (h)\033[0m\n"
  bats ${PATH_TESTS_FUNCTIONAL}/help.bats.sh
  if [ $? -ne 0 ]; then
    IS_ERROR=true
    ((AMOUNT_ERRORS=AMOUNT_ERRORS+1))
  fi

  printf "\n\033[4mTest display of version number (v)\033[0m\n"
  bats ${PATH_TESTS_FUNCTIONAL}/version.bats.sh
  if [ $? -ne 0 ]; then
    IS_ERROR=true
    ((AMOUNT_ERRORS=AMOUNT_ERRORS+1))
  fi

  # List DOCX contents:
  printf "\n\033[4mTest listing files in DOCX (ls)\033[0m\n"
  bats ${PATH_TESTS_FUNCTIONAL}/ls.bats.sh
  if [ $? -ne 0 ]; then
    IS_ERROR=true
    ((AMOUNT_ERRORS=AMOUNT_ERRORS+1))
  fi

  printf "\n\033[4mTest listing files in DOCX as JSON (lsj)\033[0m\n"
  bats ${PATH_TESTS_FUNCTIONAL}/lsj.bats.sh
  if [ $? -ne 0 ]; then
    IS_ERROR=true
    ((AMOUNT_ERRORS=AMOUNT_ERRORS+1))
  fi

  printf "\n\033[4mTest listing fields in DOCX (lsd)\033[0m\n"
  bats ${PATH_TESTS_FUNCTIONAL}/lsd.bats.sh
  if [ $? -ne 0 ]; then
    IS_ERROR=true
    ((AMOUNT_ERRORS=AMOUNT_ERRORS+1))
  fi

  printf "\n\033[4mTest listing fields in DOCX as JSON (lsdj)\033[0m\n"
  bats ${PATH_TESTS_FUNCTIONAL}/lsdj.bats.sh
  if [ $? -ne 0 ]; then
    IS_ERROR=true
    ((AMOUNT_ERRORS=AMOUNT_ERRORS+1))
  fi

  printf "\n\033[4mTest listing referenced fonts in DOCX (lsf)\033[0m\n"
  bats ${PATH_TESTS_FUNCTIONAL}/lsf.bats.sh
  if [ $? -ne 0 ]; then
    IS_ERROR=true
    ((AMOUNT_ERRORS=AMOUNT_ERRORS+1))
  fi

  printf "\n\033[4mTest listing referenced fonts in DOCX as JSON (lsfj)\033[0m\n"
  bats ${PATH_TESTS_FUNCTIONAL}/lsfj.bats.sh
  if [ $? -ne 0 ]; then
    IS_ERROR=true
    ((AMOUNT_ERRORS=AMOUNT_ERRORS+1))
  fi

  printf "\n\033[4mTest listing images in DOCX (lsi)\033[0m\n"
  bats ${PATH_TESTS_FUNCTIONAL}/lsi.bats.sh
  if [ $? -ne 0 ]; then
    IS_ERROR=true
    ((AMOUNT_ERRORS=AMOUNT_ERRORS+1))
  fi

  printf "\n\033[4mTest listing images in DOCX as JSON (lsij)\033[0m\n"
  bats ${PATH_TESTS_FUNCTIONAL}/lsij.bats.sh
  if [ $? -ne 0 ]; then
    IS_ERROR=true
    ((AMOUNT_ERRORS=AMOUNT_ERRORS+1))
  fi

  printf "\n\033[4mTest listing files containing given string (lsl)\033[0m\n"
  bats ${PATH_TESTS_FUNCTIONAL}/lsl.bats.sh
  if [ $? -ne 0 ]; then
    IS_ERROR=true
    ((AMOUNT_ERRORS=AMOUNT_ERRORS+1))
  fi

  printf "\n\033[4mTest listing files containing given string as JSON (lslj)\033[0m\n"
  bats ${PATH_TESTS_FUNCTIONAL}/lslj.bats.sh
  if [ $? -ne 0 ]; then
    IS_ERROR=true
    ((AMOUNT_ERRORS=AMOUNT_ERRORS+1))
  fi

  printf "\n\033[4mTest listing metadata in DOCX (lsm)\033[0m\n"
  bats ${PATH_TESTS_FUNCTIONAL}/lsm.bats.sh
  if [ $? -ne 0 ]; then
    IS_ERROR=true
    ((AMOUNT_ERRORS=AMOUNT_ERRORS+1))
  fi

  printf "\n\033[4mTest listing metadata in DOCX as JSON (lsmj)\033[0m\n"
  bats ${PATH_TESTS_FUNCTIONAL}/lsmj.bats.sh
  if [ $? -ne 0 ]; then
    IS_ERROR=true
    ((AMOUNT_ERRORS=AMOUNT_ERRORS+1))
  fi

  # Convert and compare DOCX:
  printf "\n\033[4mTest output XML document\033[0m\n"
  bats ${PATH_TESTS_FUNCTIONAL}/cat.bats.sh
  if [ $? -ne 0 ]; then
    IS_ERROR=true
    ((AMOUNT_ERRORS=AMOUNT_ERRORS+1))
  fi

  printf "\n\033[4mTest output DOCX document as plaintext (txt)\033[0m\n"
  bats ${PATH_TESTS_FUNCTIONAL}/txt.bats.sh
  if [ $? -ne 0 ]; then
    IS_ERROR=true
    ((AMOUNT_ERRORS=AMOUNT_ERRORS+1))
  fi

  printf "\n\033[4mTest side-by-side comparison from two DOCX archives (diff)\033[0m\n"
  bats ${PATH_TESTS_FUNCTIONAL}/diff.bats.sh
  if [ $? -ne 0 ]; then
    IS_ERROR=true
    ((AMOUNT_ERRORS=AMOUNT_ERRORS+1))
  fi

  # Manipulate DOCX:
  printf "\n\033[4mTest replacing text with dummy text in DOCX (lorem)\033[0m\n"
  bats ${PATH_TESTS_FUNCTIONAL}/lorem.bats.sh
  if [ $? -ne 0 ]; then
    IS_ERROR=true
    ((AMOUNT_ERRORS=AMOUNT_ERRORS+1))
  fi

  printf "\n\033[4mTest replacing image in DOCX (rpi)\033[0m\n"
  bats ${PATH_TESTS_FUNCTIONAL}/rpi.bats.sh
  if [ $? -ne 0 ]; then
    IS_ERROR=true
    ((AMOUNT_ERRORS=AMOUNT_ERRORS+1))
  fi

  printf "\n\033[4mTest replacing text in DOCX (rpt)\033[0m\n"
  bats ${PATH_TESTS_FUNCTIONAL}/rpt.bats.sh
  if [ $? -ne 0 ]; then
    IS_ERROR=true
    ((AMOUNT_ERRORS=AMOUNT_ERRORS+1))
  fi

  printf "\n\033[4mTest modifying or setting metadata in DOCX (mm)\033[0m\n"
  bats ${PATH_TESTS_FUNCTIONAL}/mm.bats.sh
  if [ $? -ne 0 ]; then
    IS_ERROR=true
    ((AMOUNT_ERRORS=AMOUNT_ERRORS+1))
  fi

  printf "\n\033[4mTest removing DOCX contens between given strings (rmt)\033[0m\n"
  bats ${PATH_TESTS_FUNCTIONAL}/rmt.bats.sh
  if [ $? -ne 0 ]; then
    IS_ERROR=true
    ((AMOUNT_ERRORS=AMOUNT_ERRORS+1))
  fi

  printf "\n\033[4mTest setting field value in DOCX (sfv)\033[0m\n"
  bats ${PATH_TESTS_FUNCTIONAL}/sfv.bats.sh
  if [ $? -ne 0 ]; then
    IS_ERROR=true
    ((AMOUNT_ERRORS=AMOUNT_ERRORS+1))
  fi

  # Batch processing
  printf "\n\033[4mTest processing multiple docxBox commands (batch)\033[0m\n"
  bats ${PATH_TESTS_FUNCTIONAL}/batch.bats.sh
  if [ $? -ne 0 ]; then
    IS_ERROR=true
    ((AMOUNT_ERRORS=AMOUNT_ERRORS+1))
  fi

  # Extract and create DOCX:
  printf "\n\033[4mTest unziping files from DOCX (uz)\033[0m\n"
  bats ${PATH_TESTS_FUNCTIONAL}/uz.bats.sh
  if [ $? -ne 0 ]; then
    IS_ERROR=true
    ((AMOUNT_ERRORS=AMOUNT_ERRORS+1))
  fi

  printf "\n\033[4mTest unziping files from DOCX and indenting XML files (uzi)\033[0m\n"
  bats ${PATH_TESTS_FUNCTIONAL}/uzi.bats.sh
  if [ $? -ne 0 ]; then
    IS_ERROR=true
    ((AMOUNT_ERRORS=AMOUNT_ERRORS+1))
  fi

  printf "\n\033[4mTest unziping only media files from DOCX (uzm)\033[0m\n"
  bats ${PATH_TESTS_FUNCTIONAL}/uzm.bats.sh
  if [ $? -ne 0 ]; then
    IS_ERROR=true
    ((AMOUNT_ERRORS=AMOUNT_ERRORS+1))
  fi

  printf "\n\033[4mTest creating (zp) DOCX from files (zp)\033[0m\n"
  bats ${PATH_TESTS_FUNCTIONAL}/zp.bats.sh
  if [ $? -ne 0 ]; then
    IS_ERROR=true
    ((AMOUNT_ERRORS=AMOUNT_ERRORS+1))
  fi

  printf "\n\033[4mTest creating DOCX from indented files (zpc)\033[0m\n"
  bats ${PATH_TESTS_FUNCTIONAL}/zpc.bats.sh
  if [ $? -ne 0 ]; then
    IS_ERROR=true
    ((AMOUNT_ERRORS=AMOUNT_ERRORS+1))
  fi
}

function init_valgrind() {
  IS_VALGRIND_TEST=true
  export IS_VALGRIND_TEST

  if [ ! -d $PATH_TESTS_VALGRIND ]; then
    mkdir $PATH_TESTS_VALGRIND
    cp $PATH_TESTS_FUNCTIONAL/* $PATH_TESTS_VALGRIND -r
  fi

  PATH_TESTS_FUNCTIONAL=$PATH_TESTS_VALGRIND
}

function cleanup_valgrind() {
  if [ PATH_TESTS_FUNCTIONAL == PATH_TESTS_VALGRIND ]; then
    rm -rf $PATH_TESTS_VALGRIND
  fi
}

if [ "$1" != "" ]; then
  if [ "$1" == "valgrind" ]; then
      init_valgrind
      run_all_cases
    else
      run_single_case $1
  fi
else
  run_all_cases
fi

ELAPSED_TIME=$(($SECONDS - $START_TIME))
printf "\nDone. Bats tests ran for $ELAPSED_TIME seconds.\n\n"

cleanup_valgrind

if $IS_ERROR;
then
    printf "\n${AMOUNT_ERRORS} error(s) while executing tests\n\n"
    exit 1
else
  exit 0
fi
