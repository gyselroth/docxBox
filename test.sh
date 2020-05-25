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

# Meta commands
printf "\033[4mTest display of help (h)\033[0m\n"
bats ./test/functional/help.bats.sh

printf "\n\033[4mTest display of version number (v)\033[0m\n"
bats ./test/functional/version.bats.sh

# List DOCX contents:
printf "\n\033[4mTest listing files in DOCX (ls)\033[0m\n"
bats ./test/functional/ls.bats.sh

printf "\n\033[4mTest listing files in DOCX as JSON (lsj)\033[0m\n"
bats ./test/functional/lsj.bats.sh

printf "\n\033[4mTest listing fields in DOCX (lsd)\033[0m\n"
bats ./test/functional/lsd.bats.sh

printf "\n\033[4mTest listing fields in DOCX as JSON (lsdj)\033[0m\n"
bats ./test/functional/lsdj.bats.sh

printf "\n\033[4mTest listing referenced fonts in DOCX (lsf)\033[0m\n"
bats ./test/functional/lsf.bats.sh

printf "\n\033[4mTest listing referenced fonts in DOCX as JSON (lsfj)\033[0m\n"
bats ./test/functional/lsfj.bats.sh

printf "\n\033[4mTest listing images in DOCX (lsi)\033[0m\n"
bats ./test/functional/lsi.bats.sh

printf "\n\033[4mTest listing images in DOCX as JSON (lsij)\033[0m\n"
bats ./test/functional/lsij.bats.sh

printf "\n\033[4mTest listing files containing given string (lsl)\033[0m\n"
bats ./test/functional/lsl.bats.sh

printf "\n\033[4mTest listing files containing given string as JSON (lslj)\033[0m\n"
bats ./test/functional/lslj.bats.sh

printf "\n\033[4mTest listing metadata in DOCX (lsm)\033[0m\n"
bats ./test/functional/lsm.bats.sh

printf "\n\033[4mTest listing metadata in DOCX as JSON (lsmj)\033[0m\n"
bats ./test/functional/lsmj.bats.sh

# Convert and compare DOCX:
printf "\n\033[4mTest output XML document\033[0m\n"
bats ./test/functional/cat.bats.sh

printf "\n\033[4mTest output DOCX document as plaintext (txt)\033[0m\n"
bats ./test/functional/txt.bats.sh

printf "\n\033[4mTest side-by-side comparison from two DOCX archives (diff)\033[0m\n"
bats ./test/functional/diff.bats.sh

# Manipulate DOCX:
printf "\n\033[4mTest replacing text with dummy text in DOCX (lorem)\033[0m\n"
bats ./test/functional/lorem.bats.sh

printf "\n\033[4mTest replacing image in DOCX (rpi)\033[0m\n"
bats ./test/functional/rpi.bats.sh

printf "\n\033[4mTest replacing text in DOCX (rpt)\033[0m\n"
bats ./test/functional/rpt.bats.sh

printf "\n\033[4mTest modifying or setting metadata in DOCX (mm)\033[0m\n"
bats ./test/functional/mm.bats.sh

printf "\n\033[4mTest removing DOCX contens between given strings (rmt)\033[0m\n"
bats ./test/functional/rmt.bats.sh

printf "\n\033[4mTest setting field value in DOCX (sfv)\033[0m\n"
bats ./test/functional/sfv.bats.sh

# Extract and create DOCX:
printf "\n\033[4mTest unziping files from DOCX (uz)\033[0m\n"
bats ./test/functional/uz.bats.sh

printf "\n\033[4mTest unziping files from DOCX and indenting XML files (uzi)\033[0m\n"
bats ./test/functional/uzi.bats.sh

printf "\n\033[4mTest unziping only media files from DOCX (uzm)\033[0m\n"
bats ./test/functional/uzm.bats.sh

printf "\n\033[4mTest creating (zp) DOCX from files (zp)\033[0m\n"
bats ./test/functional/zp.bats.sh

ELAPSED_TIME=$(($SECONDS - $START_TIME))
printf "\nDone. Bats tests ran for $ELAPSED_TIME seconds.\n\n"
