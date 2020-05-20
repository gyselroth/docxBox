#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

docxbox=""$BATS_TEST_DIRNAME"/docxbox"
path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"

description="only media files are extracted"
unzipped_docx="cp_table_unordered_list_images.docx-media-extracted"

@test "Output of \"docxbox uzm {missing argument}\" is an error message" {
  pattern="docxBox Error - Missing argument: Filename of DOCX to be extracted"

  run "${docxbox}" uzm
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]
}

@test "Output of \"docxbox uzm wrong_file_type\" is an error message" {
  pattern="docxBox Error - File is no ZIP archive:"
  err_log="test/functional/tmp/err.log"
  wrong_file_types=(
  "test/functional/tmp/cp_lorem_ipsum.pdf"
  "test/functional/tmp/cp_mock_csv.csv"
  "test/functional/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    "${docxbox}" uzm "${i}" 2>&1 | tee "${err_log}"
    cat "${err_log}" | grep --count "${pattern}"
  done
}

@test "With \"docxbox uzm filename.docx\" ${description}" {
  run "${docxbox}" uzm "${path_docx}"
}

@test "Unzipped files are located in project root" {
  ls | grep --count "${unzipped_docx}"

  if [ -d "${unzipped_docx}" ]; then
    rm --recursive "${unzipped_docx}";
  fi
}

@test "With \"docxbox uz filename.docx --media\" ${description}" {
  run "${docxbox}" uz "${path_docx}" --media
}

@test "Unzipped files are located in project root after running uz --media " {
  ls | grep --count "${unzipped_docx}"

  if [ -d "${unzipped_docx}" ]; then
    rm --recursive "${unzipped_docx}";
  fi
}

@test "With \"docxbox uz filename.docx -m\" ${description}" {
  run "${docxbox}" uz "${path_docx}" -m
}

@test "Unzipped files are located in project root after running uz -m" {
  ls | grep --count "${unzipped_docx}"

  if [ -d "${unzipped_docx}" ]; then
    rm --recursive "${unzipped_docx}";
  fi
}
