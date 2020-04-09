#!/usr/bin/env bats

load _helper

@test "Running w/o any command displays help" {
  run $BATS_TEST_DIRNAME/docxbox
  [ "$status" -eq 0 ]
  [ "Usage: docxbox <command> [args]" = "${lines[0]}" ]
}

@test "\"docxbox h\" displays help" {
  run $BATS_TEST_DIRNAME/docxbox h
  [ "$status" -eq 0 ]
  [ "Usage: docxbox <command> [args]" = "${lines[0]}" ]
}

@test "\"docxbox ?\" displays help" {
  run $BATS_TEST_DIRNAME/docxbox h
  [ "$status" -eq 0 ]
  [ "Usage: docxbox <command> [args]" = "${lines[0]}"  ]
}

@test "\"docxbox h ls\" displays help for ls command" {
  run $BATS_TEST_DIRNAME/docxbox h ls
  [ "$status" -eq 0 ]
  [ "Command: ls - List DOCX contents:" = "${lines[0]}" ]
}

@test "\"docxbox h lsj\" displays help for lsj command" {
  run $BATS_TEST_DIRNAME/docxbox h lsj
  [ "$status" -eq 0 ]
  [ "Command: ls - List DOCX contents:" = "${lines[0]}" ]
}

@test "\"docxbox h lsf\" displays help for lsf command" {
  run $BATS_TEST_DIRNAME/docxbox h lsf
  [ "$status" -eq 0 ]
  [ "Command: lsf - List fonts referenced in DOCX:" = "${lines[0]}" ]
}

@test "\"docxbox h lsfj\" displays help for lsfj command" {
  run $BATS_TEST_DIRNAME/docxbox h lsfj
  [ "$status" -eq 0 ]
  [ "Command: lsf - List fonts referenced in DOCX:" = "${lines[0]}" ]
}

@test "\"docxbox h lsi\" displays help for lsi command" {
  run $BATS_TEST_DIRNAME/docxbox h lsi
  [ "$status" -eq 0 ]
  [ "Command: lsf - List images in DOCX:" = "${lines[0]}" ]
}

@test "\"docxbox h lsij\" displays help for lsij command" {
  run $BATS_TEST_DIRNAME/docxbox h lsij
  [ "$status" -eq 0 ]
  [ "Command: lsf - List images in DOCX:" = "${lines[0]}" ]
}

@test "\"docxbox h lsm\" displays help for lsm command" {
  run $BATS_TEST_DIRNAME/docxbox h lsm
  [ "$status" -eq 0 ]
  [ "Command: lsm - List meta data DOCX:" = ${lines[0]} ]
}

@test "\"docxbox h lsmj\" displays help for lsmj command" {
  run $BATS_TEST_DIRNAME/docxbox h lsmj
  [ "$status" -eq 0 ]
  [ "Command: lsm - List meta data DOCX:" = "${lines[0]}" ]
}
