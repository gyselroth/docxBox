#!/bin/bash

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

################################################################################
# Run functional tests written with bats - https://github.com/sstephenson/bats #
#                                                                              #
# Install bats on Linux: sudo apt-get install bats                             #
# Install bats on Mac: brew install bats                                       #
################################################################################

START_TIME=$SECONDS
IS_ERROR=false
AMOUNT_ERRORS=0

# Meta commands
printf "\033[4mTest display of help (h)\033[0m\n"
bats ./test/functional/help.bats.sh
if [ $? -ne 0 ]; then
  IS_ERROR=true
  ((AMOUNT_ERRORS=AMOUNT_ERRORS+1))
fi

printf "\n\033[4mTest display of version number (v)\033[0m\n"
bats ./test/functional/version.bats.sh
if [ $? -ne 0 ]; then
  IS_ERROR=true
  ((AMOUNT_ERRORS=AMOUNT_ERRORS+1))
fi

# List DOCX contents:
printf "\n\033[4mTest listing files in DOCX (ls)\033[0m\n"
bats ./test/functional/ls.bats.sh
if [ $? -ne 0 ]; then
  IS_ERROR=true
  ((AMOUNT_ERRORS=AMOUNT_ERRORS+1))
fi

printf "\n\033[4mTest listing files in DOCX as JSON (lsj)\033[0m\n"
bats ./test/functional/lsj.bats.sh
if [ $? -ne 0 ]; then
  IS_ERROR=true
  ((AMOUNT_ERRORS=AMOUNT_ERRORS+1))
fi

printf "\n\033[4mTest listing fields in DOCX (lsd)\033[0m\n"
bats ./test/functional/lsd.bats.sh
if [ $? -ne 0 ]; then
  IS_ERROR=true
  ((AMOUNT_ERRORS=AMOUNT_ERRORS+1))
fi

printf "\n\033[4mTest listing fields in DOCX as JSON (lsdj)\033[0m\n"
bats ./test/functional/lsdj.bats.sh
if [ $? -ne 0 ]; then
  IS_ERROR=true
  ((AMOUNT_ERRORS=AMOUNT_ERRORS+1))
fi

printf "\n\033[4mTest listing referenced fonts in DOCX (lsf)\033[0m\n"
bats ./test/functional/lsf.bats.sh
if [ $? -ne 0 ]; then
  IS_ERROR=true
  ((AMOUNT_ERRORS=AMOUNT_ERRORS+1))
fi

printf "\n\033[4mTest listing referenced fonts in DOCX as JSON (lsfj)\033[0m\n"
bats ./test/functional/lsfj.bats.sh
if [ $? -ne 0 ]; then
  IS_ERROR=true
  ((AMOUNT_ERRORS=AMOUNT_ERRORS+1))
fi

printf "\n\033[4mTest listing images in DOCX (lsi)\033[0m\n"
bats ./test/functional/lsi.bats.sh
if [ $? -ne 0 ]; then
  IS_ERROR=true
  ((AMOUNT_ERRORS=AMOUNT_ERRORS+1))
fi

printf "\n\033[4mTest listing images in DOCX as JSON (lsij)\033[0m\n"
bats ./test/functional/lsij.bats.sh
if [ $? -ne 0 ]; then
  IS_ERROR=true
  ((AMOUNT_ERRORS=AMOUNT_ERRORS+1))
fi

printf "\n\033[4mTest listing files containing given string (lsl)\033[0m\n"
bats ./test/functional/lsl.bats.sh
if [ $? -ne 0 ]; then
  IS_ERROR=true
  ((AMOUNT_ERRORS=AMOUNT_ERRORS+1))
fi

printf "\n\033[4mTest listing files containing given string as JSON (lslj)\033[0m\n"
bats ./test/functional/lslj.bats.sh
if [ $? -ne 0 ]; then
  IS_ERROR=true
  ((AMOUNT_ERRORS=AMOUNT_ERRORS+1))
fi

printf "\n\033[4mTest listing metadata in DOCX (lsm)\033[0m\n"
bats ./test/functional/lsm.bats.sh
if [ $? -ne 0 ]; then
  IS_ERROR=true
  ((AMOUNT_ERRORS=AMOUNT_ERRORS+1))
fi

printf "\n\033[4mTest listing metadata in DOCX as JSON (lsmj)\033[0m\n"
bats ./test/functional/lsmj.bats.sh
if [ $? -ne 0 ]; then
  IS_ERROR=true
  ((AMOUNT_ERRORS=AMOUNT_ERRORS+1))
fi

# Convert and compare DOCX:
printf "\n\033[4mTest output XML document\033[0m\n"
bats ./test/functional/cat.bats.sh
if [ $? -ne 0 ]; then
  IS_ERROR=true
  ((AMOUNT_ERRORS=AMOUNT_ERRORS+1))
fi

printf "\n\033[4mTest output DOCX document as plaintext (txt)\033[0m\n"
bats ./test/functional/txt.bats.sh
if [ $? -ne 0 ]; then
  IS_ERROR=true
  ((AMOUNT_ERRORS=AMOUNT_ERRORS+1))
fi

printf "\n\033[4mTest side-by-side comparison from two DOCX archives (diff)\033[0m\n"
bats ./test/functional/diff.bats.sh
if [ $? -ne 0 ]; then
  IS_ERROR=true
  ((AMOUNT_ERRORS=AMOUNT_ERRORS+1))
fi

# Manipulate DOCX:
printf "\n\033[4mTest replacing text with dummy text in DOCX (lorem)\033[0m\n"
bats ./test/functional/lorem.bats.sh
if [ $? -ne 0 ]; then
  IS_ERROR=true
  ((AMOUNT_ERRORS=AMOUNT_ERRORS+1))
fi

printf "\n\033[4mTest replacing image in DOCX (rpi)\033[0m\n"
bats ./test/functional/rpi.bats.sh
if [ $? -ne 0 ]; then
  IS_ERROR=true
  ((AMOUNT_ERRORS=AMOUNT_ERRORS+1))
fi

printf "\n\033[4mTest replacing text in DOCX (rpt)\033[0m\n"
bats ./test/functional/rpt.bats.sh
if [ $? -ne 0 ]; then
  IS_ERROR=true
  ((AMOUNT_ERRORS=AMOUNT_ERRORS+1))
fi

printf "\n\033[4mTest modifying or setting metadata in DOCX (mm)\033[0m\n"
bats ./test/functional/mm.bats.sh
if [ $? -ne 0 ]; then
  IS_ERROR=true
  ((AMOUNT_ERRORS=AMOUNT_ERRORS+1))
fi

printf "\n\033[4mTest removing DOCX contens between given strings (rmt)\033[0m\n"
bats ./test/functional/rmt.bats.sh
if [ $? -ne 0 ]; then
  IS_ERROR=true
  ((AMOUNT_ERRORS=AMOUNT_ERRORS+1))
fi

printf "\n\033[4mTest setting field value in DOCX (sfv)\033[0m\n"
bats ./test/functional/sfv.bats.sh
if [ $? -ne 0 ]; then
  IS_ERROR=true
  ((AMOUNT_ERRORS=AMOUNT_ERRORS+1))
fi

# Extract and create DOCX:
printf "\n\033[4mTest unziping files from DOCX (uz)\033[0m\n"
bats ./test/functional/uz.bats.sh
if [ $? -ne 0 ]; then
  IS_ERROR=true
  ((AMOUNT_ERRORS=AMOUNT_ERRORS+1))
fi

printf "\n\033[4mTest unziping files from DOCX and indenting XML files (uzi)\033[0m\n"
bats ./test/functional/uzi.bats.sh
if [ $? -ne 0 ]; then
  IS_ERROR=true
  ((AMOUNT_ERRORS=AMOUNT_ERRORS+1))
fi

printf "\n\033[4mTest unziping only media files from DOCX (uzm)\033[0m\n"
bats ./test/functional/uzm.bats.sh
if [ $? -ne 0 ]; then
  IS_ERROR=true
  ((AMOUNT_ERRORS=AMOUNT_ERRORS+1))
fi

printf "\n\033[4mTest creating (zp) DOCX from files (zp)\033[0m\n"
bats ./test/functional/zp.bats.sh
if [ $? -ne 0 ]; then
  IS_ERROR=true
  ((AMOUNT_ERRORS=AMOUNT_ERRORS+1))
fi

printf "\n\033[4mTest creating DOCX from indented files (zpc)\033[0m\n"
bats ./test/functional/zpc.bats.sh
if [ $? -ne 0 ]; then
  IS_ERROR=true
  ((AMOUNT_ERRORS=AMOUNT_ERRORS+1))
fi

ELAPSED_TIME=$(($SECONDS - $START_TIME))
printf "\nDone. Bats tests ran for $ELAPSED_TIME seconds.\n\n"

if $IS_ERROR;
then
    printf "\n${AMOUNT_ERRORS} error(s) while executing tests\n\n"
    exit 1
else
  exit 0
fi
