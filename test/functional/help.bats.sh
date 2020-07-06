#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

VALGRIND_LOG="test/tmp/mem-leak.log"
VALGRIND="valgrind -v --leak-check=full\
 --log-file=${VALGRIND_LOG}"

VALGRIND_ERR_PATTERN="ERROR SUMMARY: 1 errors from 1 contexts"

if $IS_VALGRIND_TEST;
then
  DOCXBOX_BINARY="${VALGRIND} $BATS_TEST_DIRNAME/../tmp/docxbox"
else
  DOCXBOX_BINARY="$BATS_TEST_DIRNAME/../tmp/docxbox"
fi

REGEX_VERSION_CHECK="(^|\s)+(docxBox v)\K([0-9]|\.)*(?=\s|$)"

# Meta commands
@test "Running w/o any command displays help" {
  ${DOCXBOX_BINARY} | grep "Usage: docxbox <command> \[args\]"

  checkForValgrindError
}

@test "Running w/o any command displays version" {
  ${DOCXBOX_BINARY} | grep --perl-regexp --only-matching "${REGEX_VERSION_CHECK}"

  checkForValgrindError
}

@test "\"docxbox h\" displays help" {
  ${DOCXBOX_BINARY} h | grep "Usage: docxbox <command> \[args\]"

  checkForValgrindError
}

@test "\"docxbox h\" displays version" {
  ${DOCXBOX_BINARY} h | grep --perl-regexp --only-matching "${REGEX_VERSION_CHECK}"

  checkForValgrindError
}

@test "\"docxbox ?\" displays help" {
  ${DOCXBOX_BINARY} ? | grep "Usage: docxbox <command> \[args\]"

  checkForValgrindError
}

@test "\"docxbox ?\" displays version" {
  ${DOCXBOX_BINARY} ? | grep --perl-regexp --only-matching "${REGEX_VERSION_CHECK}"

  checkForValgrindError
}

# List DOCX contents:
@test "\"docxbox h ls\" displays help for ls command" {
  run ${DOCXBOX_BINARY} h ls
  [ "$status" -eq 0 ]
  [ "Command: ls - List DOCX contents:" = "${lines[0]}" ]

  checkForValgrindError
}

@test "\"docxbox h lsj\" displays help for lsj command" {
  run ${DOCXBOX_BINARY} h lsj
  [ "$status" -eq 0 ]
  [ "Command: ls - List DOCX contents:" = "${lines[0]}" ]

  checkForValgrindError
}

@test "\"docxbox h lsl\" displays help for lsl command" {
  pattern="List all files containing search-string or regular expression:"

  run ${DOCXBOX_BINARY} h lsl
  [ "$status" -eq 0 ]
  [ "${pattern}" = "${lines[0]}" ]

  checkForValgrindError
}

@test "\"docxbox h lslj\" displays help for lslj command" {
  pattern="List all files containing search-string or regular expression:"

  run ${DOCXBOX_BINARY} h lslj
  [ "$status" -eq 0 ]
  [ "${pattern}" = "${lines[0]}" ]

  checkForValgrindError
}

@test "\"docxbox h lsf\" displays help for lsf command" {
  run ${DOCXBOX_BINARY} h lsf
  [ "$status" -eq 0 ]
  [ "Command: lsf - List fonts referenced in DOCX:" = "${lines[0]}" ]

  checkForValgrindError
}

@test "\"docxbox h lsfj\" displays help for lsfj command" {
  run ${DOCXBOX_BINARY} h lsfj
  [ "$status" -eq 0 ]
  [ "Command: lsf - List fonts referenced in DOCX:" = "${lines[0]}" ]

  checkForValgrindError
}

@test "\"docxbox h lsd\" displays help for lsd command" {
  run ${DOCXBOX_BINARY} h lsd
  [ "$status" -eq 0 ]
  [ "Command: lsd - List fields from DOCX:" = "${lines[0]}" ]

  checkForValgrindError
}

@test "\"docxbox h lsdj\" displays help for lsdj command" {
  run ${DOCXBOX_BINARY} h lsdj
  [ "$status" -eq 0 ]
  [ "Command: lsd - List fields from DOCX:" = "${lines[0]}" ]

  checkForValgrindError
}

@test "\"docxbox h lsi\" displays help for lsi command" {
  run ${DOCXBOX_BINARY} h lsi
  [ "$status" -eq 0 ]
  [ "Command: lsf - List images in DOCX:" = "${lines[0]}" ]

  checkForValgrindError
}

@test "\"docxbox h lsij\" displays help for lsij command" {
  run ${DOCXBOX_BINARY} h lsij
  [ "$status" -eq 0 ]
  [ "Command: lsf - List images in DOCX:" = "${lines[0]}" ]

  checkForValgrindError
}

@test "\"docxbox h lsm\" displays help for lsm command" {
  run ${DOCXBOX_BINARY} h lsm
  [ "$status" -eq 0 ]
  [ "Command: lsm - List meta data of DOCX:" = "${lines[0]}" ]

  checkForValgrindError
}

@test "\"docxbox h lsmj\" displays help for lsmj command" {
  run ${DOCXBOX_BINARY} h lsmj
  [ "$status" -eq 0 ]
  [ "Command: lsm - List meta data of DOCX:" = "${lines[0]}" ]

  checkForValgrindError
}

# Manipulate DOCX document:
@test "\"docxbox h rpi\" displays help for rpi command" {
  run ${DOCXBOX_BINARY} h rpi
  [ "$status" -eq 0 ]
  [ "Command: rpi - Replace image in DOCX document:" = "${lines[0]}" ]

  checkForValgrindError
}

@test "\"docxbox h rpt\" displays help for rpt command" {
  run ${DOCXBOX_BINARY} h rpt
  [ "$status" -eq 0 ]
  [ "Command: rpt - Replace text in DOCX document:" = "${lines[0]}" ]

  checkForValgrindError
}

@test "\"docxbox h rmt\" displays help for rmt command" {
  pattern="Command: rmt - Remove DOCX content between given strings:"

  run ${DOCXBOX_BINARY} h rmt
  [ "$status" -eq 0 ]
  [ "${pattern}" = "${lines[0]}" ]

  checkForValgrindError
}

@test "\"docxbox h mm\" displays help for mm command" {
  run ${DOCXBOX_BINARY} h mm
  [ "$status" -eq 0 ]
  [ "Command: mm - Modify or set meta attribute in DOCX:" = "${lines[0]}" ]

  checkForValgrindError
}

@test "\"docxbox h sfv\" displays help for sfv command" {
  run ${DOCXBOX_BINARY} h sfv
  [ "$status" -eq 0 ]
  [ "Command: sfv - Set field value:" = "${lines[0]}" ]

  checkForValgrindError
}

# Convert DOCX:
@test "\"docxbox h txt\" displays help for txt command" {
  run ${DOCXBOX_BINARY} h txt
  [ "$status" -eq 0 ]
  [ "Command: txt - Output plaintext from DOCX document:" = "${lines[0]}" ]

  checkForValgrindError
}

@test "\"docxbox h diff\" displays help for diff command" {
  pattern="Command diff - Side-by-side compare file from two DOCX archives:"

  run ${DOCXBOX_BINARY} h diff
  [ "$status" -eq 0 ]
  [ "${pattern}" = "${lines[0]}" ]

  checkForValgrindError
}

# Batch process
@test "\"docxbox h batch\" displays help for batch command" {
  pattern="Command batch - Process multiple docxBox commands upon given DOCX"

  run ${DOCXBOX_BINARY} h batch
  [ "$status" -eq 0 ]
  [ "${pattern}" = "${lines[0]}" ]

  checkForValgrindError
}

# Extract and create DOCX:
@test "\"docxbox h uz\" displays help for uz command" {
  run ${DOCXBOX_BINARY} h uz
  [ "$status" -eq 0 ]
  [ "Command: uz - Unzip given DOCX file:" = "${lines[0]}" ]

  checkForValgrindError
}

@test "\"docxbox h uzi\" displays help for uzi command" {
  run ${DOCXBOX_BINARY} h uzi
  [ "$status" -eq 0 ]
  [ "Command: uzi - Unzip DOCX and indent XML files:" = "${lines[0]}" ]

  checkForValgrindError
}

@test "\"docxbox h uzm\" displays help for uzm command" {
  run ${DOCXBOX_BINARY} h uzm
  [ "$status" -eq 0 ]
  [ "Command: uzm - Unzip only media files DOCX file:" = "${lines[0]}" ]

  checkForValgrindError
}

@test "\"docxbox h zp\" displays help for zp command" {
  run ${DOCXBOX_BINARY} h zp
  [ "$status" -eq 0 ]
  [ "Command: zp - Create (zip) DOCX from files:" = "${lines[0]}" ]

  checkForValgrindError
}

@test "\"docxbox h zpc\" displays help for zpc command" {
  pattern="Command: zpc - Compress XML, than create DOCX from files:"

  run ${DOCXBOX_BINARY} h zpc
  [ "$status" -eq 0 ]
  [ "${pattern}" = "${lines[0]}" ]

  checkForValgrindError
}

checkForValgrindError() {
  if $IS_VALGRIND_TEST; then
    cat "${VALGRIND_LOG}" | grep --count --invert-match "${VALGRIND_ERR_PATTERN}"
  fi
}
