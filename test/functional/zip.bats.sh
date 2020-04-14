#!/usr/bin/env bats

load _helper

@test "Output of \"docxbox zip {missing argument}\" is an error message" {
  run "$BATS_TEST_DIRNAME"/docxbox zip
  [ "$status" -ne 0 ]
  [ "Missing argument: path of directory to be zipped" = "${lines[0]}" ]
}
