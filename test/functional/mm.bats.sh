#!/usr/bin/env bats

load _helper

@test "Output of \"docxbox mm {missing argument}\" is an error message" {
  run $BATS_TEST_DIRNAME/docxbox mm
  [ "$status" -ne 0 ]
  [ "Missing argument: Meta attribute to be set" = "${lines[0]}" ]
}
