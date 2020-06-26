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

unzipped_folder="cp_bio_assay.docx-extracted"

@test "Output of \"docxbox uz {missing argument}\" is an error message" {
  pattern="docxBox Error - Missing argument: Filename of DOCX to be extracted"

  run ${DOCXBOX_BINARY} uz
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]
}

@test "Output of \"docxbox uz nonexistent.docx\" is an error message" {
  err_log="test/tmp/err.log"

  run ${DOCXBOX_BINARY} uz nonexistent.docx
  [ "$status" -ne 0 ]

  ${DOCXBOX_BINARY} uz nonexistent.docx 2>&1 | tee "${err_log}"
  cat "${err_log}" | grep --count "docxBox Error - File not found:"
}

@test "Output of \"docxbox uz wrong_file_type\" is an error message" {
  pattern="docxBox Error - File is no ZIP archive:"
  err_log="test/tmp/err.log"
  wrong_file_types=(
  "test/tmp/cp_lorem_ipsum.pdf"
  "test/tmp/cp_mock_csv.csv"
  "test/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    ${DOCXBOX_BINARY} uz "${i}" 2>&1 | tee "${err_log}"
    cat "${err_log}" | grep --count "${pattern}"
  done
}

@test "With \"docxbox uz filename.docx\" all files are unziped" {
  pattern="^[[:space:]]\{4\}"

  run ${DOCXBOX_BINARY} uz test/tmp/cp_bio_assay.docx
  [ "$status" -eq 0 ]

  cat "${unzipped_folder}/word/document.xml" | grep --invert-match "${pattern}"
}

@test "Unziped files are located in project root" {
  ls | grep --count "${unzipped_folder}"

  if [ -d "${unzipped_folder}" ]; then
    rm --recursive "${unzipped_folder}";
  fi
}
