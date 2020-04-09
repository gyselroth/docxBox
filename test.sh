#!/bin/bash

########################################################################################################################
# Run functional tests written with bats - https://github.com/sstephenson/bats                                         #
#                                                                                                                      #
# Install bats on Linux: sudo apt-get install bats                                                                     #
# Install bats on Mac: brew install bats                                                                               #
########################################################################################################################

START_TIME=$SECONDS

printf "\033[4mTest display of help\033[0m\n"
bats ./test/functional/help.bats.sh

printf "\n\033[4mTest display of version number\033[0m\n"
bats ./test/functional/version.bats.sh

printf "\n\033[4mTest listing files in DOCX\033[0m\n"
bats ./test/functional/ls.bats.sh

printf "\n\033[4mTest listing files in DOCX as JSON\033[0m\n"
bats ./test/functional/lsj.bats.sh

printf "\n\033[4mTest listing referenced fonts in DOCX\033[0m\n"
bats ./test/functional/lsf.bats.sh

printf "\n\033[4mTest listing referenced fonts in DOCX as JSON\033[0m\n"
bats ./test/functional/lsfj.bats.sh

printf "\n\033[4mTest listing images in DOCX\033[0m\n"
bats ./test/functional/lsi.bats.sh

printf "\n\033[4mTest listing images in DOCX as JSON\033[0m\n"
bats ./test/functional/lsij.bats.sh

printf "\n\033[4mTest listing metadata in DOCX\033[0m\n"
bats ./test/functional/lsm.bats.sh

printf "\n\033[4mTest listing metadata in DOCX as JSON\033[0m\n"
bats ./test/functional/lsmj.bats.sh

ELAPSED_TIME=$(($SECONDS - $START_TIME))
printf "\nDone. Bats tests ran for $ELAPSED_TIME seconds.\n\n";
