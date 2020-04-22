#!/usr/bin/env bats

load _helper

@test "Output of \"docxbox zip {missing argument}\" is an error message" {
  run "$BATS_TEST_DIRNAME"/docxbox zip
  [ "$status" -ne 0 ]
  [ "Missing argument: path of directory to be zipped" = "${lines[0]}" ]
}

@test "With \"docxbox zip directory /path-to-file/filename.docx\" a directory can be zipped into a docx" {
  if [ ! -d test/files/docx/unziped ]; then
    mkdir test/files/docx/unziped;
    unzip test/functional/tmp/cp_table_unordered_list_images.docx -d test/files/docx/unziped;
  fi

  run "$BATS_TEST_DIRNAME"/docxbox zip test/files/docx/unziped test/files/docx/zp_table_unordered_list_images.docx
  [ "$status" -eq 0 ]
}

@test "Zipped file is located in project root" {
  ls test/files/docx | grep -c zp_table_unordered_list_images.docx

# cleanup
  if [ -d test/files/docx/unziped ]; then
    rm -r test/files/docx/unziped;
  fi

  if [ -f test/files/docx/zp_table_unordered_list_images.docx ]; then
    rm test/files/docx/zp_table_unordered_list_images.docx;
  fi
}
