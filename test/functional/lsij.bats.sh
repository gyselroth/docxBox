#!/usr/bin/env bats

load _helper

@test "Output of \"docxbox lsij {missing argument}\" is an error message" {
  run $BATS_TEST_DIRNAME/docxbox lsij
  [ "$status" -eq 125 ]
  [ "Missing argument: Filename of DOCX to be extracted" = "${lines[0]}" ]
}
