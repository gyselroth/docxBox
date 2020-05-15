#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

docxbox=""$BATS_TEST_DIRNAME"/docxbox"
path_docx="test/functional/tmp/cp_bio_assay.docx"

base_command="docxbox mm filename.docx"

@test "Output of \"docxbox mm {missing argument}\" is an error message" {
  run "${docxbox}" mm
  [ "$status" -ne 0 ]
  [ "Missing argument: DOCX filename" = "${lines[0]}" ]
}

@test "Output of \"${base_command} {missing argument}\" is an error message" {
  run "${docxbox}" mm "${path_docx}"
  [ "$status" -ne 0 ]
  [ "Missing argument: Meta attribute to be set" = "${lines[0]}" ]
}

title="the meta attribute title can be changed"
@test "With \"${base_command} title {argument}\" ${title}" {
  run "${docxbox}" mm "${path_docx}" title "someTitle"
  [ "$status" -eq 0 ]
  "${docxbox}" lsm "${path_docx}" | grep --count "title: someTitle"
}

creator="the meta attribute creator can be changed"
@test "With \"${base_command} creator {argument}\" ${creator}" {
  run "${docxbox}" mm "${path_docx}" creator "someOne"
  [ "$status" -eq 0 ]
  "${docxbox}" lsm "${path_docx}" | grep --count "creator: someOne"
}

last_modified_by="the meta attribute lastModifiedBy can be changed"
@test "With \"${base_command} lastModifiedBy {argument}\" ${last_modified_by}" {
  pattern="lastModifiedBy: someOneElse"

  run "${docxbox}" mm "${path_docx}" lastModifiedBy "someOneElse"
  [ "$status" -eq 0 ]
  "${docxbox}" lsm "${path_docx}" | grep --count "${pattern}"
}

last_printed="the meta attribute lastPrinted can be changed"
@test "With \"${base_command} lastPrinted {argument}\" ${last_printed}" {
  print_date="2020-02-20T10:31:00Z"
  pattern="lastPrinted: 2020-02-20T10:31:00Z"

  run "${docxbox}" mm "${path_docx}" lastPrinted $print_date
  [ "$status" -eq 0 ]
  "${docxbox}" lsm "${path_docx}" | grep --count "${pattern}"
}

language="the meta attribute language can be changed"
@test "With \"${base_command} language {argument}\" ${language}" {
  run "${docxbox}" mm "${path_docx}" language "de-CH"
  [ "$status" -eq 0 ]
  "${docxbox}" lsm "${path_docx}" | grep --count "language: de-CH"
}

modified="the meta attribute modified can be changed"
@test "With \"${base_command} modified {argument}\" ${modified}" {
  pattern="modified: 2020-10-20T10:20:00Z"

  run "${docxbox}" mm "${path_docx}" modified "2020-10-20T10:20:00Z"
  [ "$status" -eq 0 ]
  "${docxbox}" lsm "${path_docx}" | grep --count "${pattern}"
}

created="the meta attribute created can be changed"
@test "With \"${base_command} created {argument}\" ${created}" {
  pattern="created: 2020-10-20T10:20:00Z"

  run "${docxbox}" mm "${path_docx}" created "2020-10-20T10:20:00Z"
  [ "$status" -eq 0 ]
  "${docxbox}" lsm "${path_docx}" | grep --count "${pattern}"
}

revision="the meta attribute revision can be changed"
@test "With \"${base_command} revision {argument}\" ${revision}" {
  run "${docxbox}" mm "${path_docx}" revision "25"
  [ "$status" -eq 0 ]
  "${docxbox}" lsm "${path_docx}" | grep "revision: 25"
}
