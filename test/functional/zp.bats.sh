#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

VALGRIND="valgrind -v --leak-check=full\
 --log-file=test/assets/documents/other/mem-leak.log"

if $IS_VALGRIND_TEST;
then
  DOCXBOX_BINARY="${VALGRIND} $BATS_TEST_DIRNAME/../tmp/docxbox"
else
  DOCXBOX_BINARY="$BATS_TEST_DIRNAME/../tmp/docxbox"
fi

unzipped_docx_directory="test/tmp/unzipped"

@test "Output of \"docxbox zp {missing argument}\" is an error message" {
  pattern="docxBox Error - Missing argument: Path of directory to be zipped"

  run ${DOCXBOX_BINARY} zp
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]
}

@test "Output of \"docxbox zp directory {missing argument}\" is an error message" {
  pattern="docxBox Error - Missing argument: Filename of docx to be created"

  run ${DOCXBOX_BINARY} zp "${unzipped_docx_directory}"
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]
}

title="With \"docxbox zp directory /path-to-file/filename.docx\" "
title+="a directory can be zipped into a docx"
@test "$title" {
  if [ ! -d "${unzipped_docx_directory}" ]; then
    mkdir "${unzipped_docx_directory}";
    path_docx="test/tmp/cp_table_unordered_list_images.docx"
    unzip $path_docx -d "${unzipped_docx_directory}";
  fi

  path_new_docx="test/tmp/zp_table_unordered_list_images.docx"

  run ${DOCXBOX_BINARY} zp "${unzipped_docx_directory}" "${path_new_docx}"
  [ "$status" -eq 0 ]

  ls test/tmp | grep -c zp_table_unordered_list_images.docx
}
