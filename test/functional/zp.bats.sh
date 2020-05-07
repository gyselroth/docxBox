#!/usr/bin/env bats

load _helper

@test "Output of \"docxbox zp {missing argument}\" is an error message" {
  run "$BATS_TEST_DIRNAME"/docxbox zp
  [ "$status" -ne 0 ]
  [ "Missing argument: Path of directory to be zipped" = "${lines[0]}" ]
}

@test "With \"docxbox zp directory /path-to-file/filename.docx\" a directory can be zipped into a docx" {
  if [ ! -d test/functional/tmp/unziped ]; then
    mkdir test/functional/tmp/unziped;
    unzip test/functional/tmp/cp_table_unordered_list_images.docx -d test/functional/tmp/unziped;
  fi

  run "$BATS_TEST_DIRNAME"/docxbox zp test/functional/tmp/unziped test/functional/tmp/zp_table_unordered_list_images.docx
  [ "$status" -eq 0 ]

  ls test/functional/tmp | grep -c zp_table_unordered_list_images.docx
}
