#!/usr/bin/env bats

load _helper

@test "someting"{
  skip
run grep -c '12 files' $BATS_TEST_DIRNAME/docxbox ls files/sample_100kB.docx
}
