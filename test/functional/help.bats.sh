#!/usr/bin/env bats

load _helper

@test "Running w/o any command displays help" {
  run $BATS_TEST_DIRNAME/docxbox
  [ "$status" -eq 0 ]
  [ "${lines[0]}" = "Usage: docxbox <command> [args]" ]
}

@test "\"docxbox h\" displays help" {
  run $BATS_TEST_DIRNAME/docxbox h
  [ "$status" -eq 0 ]
  [ "${lines[0]}" = "Usage: docxbox <command> [args]" ]
}

@test "\"docxbox ?\" displays help" {
  run $BATS_TEST_DIRNAME/docxbox h
  [ "$status" -eq 0 ]
  [ "${lines[0]}" = "Usage: docxbox <command> [args]" ]
}

@test "\"docxbox h ls\" displays help for ls command" {
  run $BATS_TEST_DIRNAME/docxbox h ls
  [ "$status" -eq 0 ]
  [ "${lines[0]}" = "Command: ls - List DOCX contents:" ]
}

@test "\"docxbox h lsm\" displays help for lsm command" {
  run $BATS_TEST_DIRNAME/docxbox h lsm
  [ "$status" -eq 0 ]
  [ "${lines[0]}" = "Command: lsm - List meta data DOCX:"]
}

#@test "\"docxbox h lsmj\" displays help for lsmj command" {
#  run $BATS_TEST_DIRNAME/docxbox h lsmj
#  [ "$status" -eq 0 ]
#  [ "${lines[0]}" = "Command: lsm - List meta data DOCX:"]
#}
