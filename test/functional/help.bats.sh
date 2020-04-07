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
