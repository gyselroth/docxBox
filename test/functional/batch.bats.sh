#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

docxbox="$BATS_TEST_DIRNAME/docxbox"
path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"
base_command="docxbox batch filename.docx"

@test "Output of \"docxbox batch {missing argument}\" is an error message" {
  run "${docxbox}" batch
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: DOCX file" = "${lines[0]}" ]
}

@test "Output of \"${base_command} {missing argument}\" is an error message" {
  run "${docxbox}" batch "${path_docx}"
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: Batch commands JSON" = "${lines[0]}" ]
}

appendix="a batch sequence can be executed"
@test "With \"${base_command} batch_sequence_as_JSON\" ${appendix} and the title gets changed" {
  batch="{\"1\":{\"mm\":[\"title\",\"foo\"]},\"2\":{\"rpt\":[\"bar\",\"baz\"]},\"3\":{\"rpt\":[\"qux\",{\"h1\":{\"text\":\"Quux\"}}]}}"
  run "${docxbox}" batch "${path_docx}" "${batch}"

  "${docxbox}" lsm ${path_docx} | grep --count "title: foo"
}

@test "With \"${base_command} batch_sequence_as_JSON\" ${appendix} and a string can be replaced" {
  batch="{\"1\":{\"mm\":[\"title\",\"foo\"]},\"2\":{\"rpt\":[\"bar\",\"baz\"]},\"3\":{\"rpt\":[\"qux\",{\"h1\":{\"text\":\"Quux\"}}]}}"
  run "${docxbox}" batch "${path_docx}" "${batch}"

  "${docxbox}" txt ${path_docx} | grep --count "baz"
}

@test "With \"${base_command} batch_sequence_as_JSON\" ${appendix} and a string can be replaced by a h1" {
  batch="{\"1\":{\"mm\":[\"title\",\"foo\"]},\"2\":{\"rpt\":[\"Officia\",\"baz\"]},\"3\":{\"rpt\":[\"Lorem\",{\"h1\":{\"text\":\"Quux\"}}]}}"
  run "${docxbox}" batch "${path_docx}" "${batch}"

  "${docxbox}" cat "${path_docx}" "${display_file}" | grep --count "<w:pStyle w:val=\"para1\"/>"
}
