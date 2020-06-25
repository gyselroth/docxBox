#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

DOCXBOX_BINARY="$BATS_TEST_DIRNAME/docxbox"

PATH_DOCX="test/tmp/cp_table_unordered_list_images.docx"
BASE_COMMAND="docxbox batch filename.docx"

@test "Output of \"docxbox batch {missing argument}\" is an error message" {
  run "${DOCXBOX_BINARY}" batch
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: DOCX file" = "${lines[0]}" ]
}

@test "Output of \"${BASE_COMMAND} {missing argument}\" is an error message" {
  run "${DOCXBOX_BINARY}" batch "${PATH_DOCX}"
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: Batch commands JSON" = "${lines[0]}" ]
}

appendix="a batch sequence can be executed"
@test "With \"${BASE_COMMAND} batch_sequence_as_JSON\" ${appendix} and the title gets changed" {
  batch="{\"1\":{\"mm\":[\"title\",\"foo\"]},\"2\":{\"rpt\":[\"bar\",\"baz\"]},\"3\":{\"rpt\":[\"qux\",{\"h1\":{\"text\":\"Quux\"}}]}}"
  run "${DOCXBOX_BINARY}" batch "${PATH_DOCX}" "${batch}"

  "${DOCXBOX_BINARY}" lsm ${PATH_DOCX} | grep --count "title: foo"
}

@test "With \"${BASE_COMMAND} batch_sequence_as_JSON\" ${appendix} and a string can be replaced" {
  batch="{\"1\":{\"mm\":[\"title\",\"foo\"]},\"2\":{\"rpt\":[\"bar\",\"baz\"]},\"3\":{\"rpt\":[\"qux\",{\"h1\":{\"text\":\"Quux\"}}]}}"
  run "${DOCXBOX_BINARY}" batch "${PATH_DOCX}" "${batch}"

  "${DOCXBOX_BINARY}" txt ${PATH_DOCX} | grep --count "baz"
}

@test "With \"${BASE_COMMAND} batch_sequence_as_JSON\" ${appendix} and a string can be replaced by a h1" {
  batch="{\"1\":{\"mm\":[\"title\",\"foo\"]},\"2\":{\"rpt\":[\"Officia\",\"baz\"]},\"3\":{\"rpt\":[\"Lorem\",{\"h1\":{\"text\":\"Quux\"}}]}}"
  run "${DOCXBOX_BINARY}" batch "${PATH_DOCX}" "${batch}"

  "${DOCXBOX_BINARY}" cat "${PATH_DOCX}" "${display_file}" | grep --count "<w:pStyle w:val=\"para1\"/>"
}
