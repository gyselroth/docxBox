#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

path_docx="test/functional/tmp/cp_mergefields.docx"

base_command="docxbox sfv"
mergefield="MERGEFIELD  Schueler_Anrede"

@test "Output of \"${base_command} {missing argument}\" is an error message" {
  run "$BATS_TEST_DIRNAME"/docxbox sfv
  [ "$status" -ne 0 ]
  [ "Missing argument: Filename of DOCX to be extracted" = "${lines[0]}" ]
}

missing_arguments="filename.docx {missing argument}"
@test "Output of \"${base_command} ${missing_arguments}\" is an error message" {
  run "$BATS_TEST_DIRNAME"/docxbox sfv $path_docx
  [ "$status" -ne 0 ]
  [ "Missing argument: Field identifier" = "${lines[0]}" ]
}

missing_value="filename.docx fieldIdentifier {missing argument}"
@test "Output of \"${base_command} ${missing_value}\" is an error message" {
  run "$BATS_TEST_DIRNAME"/docxbox sfv $path_docx "$mergefield"
  [ "$status" -ne 0 ]
  [ "Missing argument: Value to be set" = "${lines[0]}" ]
}

arguments="filename.docx fieldIdentifier fieldValue"
appendix="changes the value of the given field"
@test "Output of \"${base_command} ${arguments}\" ${appendix}" {
  run "$BATS_TEST_DIRNAME"/docxbox sfv $path_docx "$mergefield" foobar
  [ "$status" -eq 0 ]

  "$BATS_TEST_DIRNAME"/docxbox txt $path_docx | grep -c "foobar"
}
