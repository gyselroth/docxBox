#!/usr/bin/env bats

load _helper

@test "\"docxbox ls filename.docx\" outputs headlines of files and directories attributes" {
  run grep -c 'Length' $BATS_TEST_DIRNAME/docxbox ls ../files/file-sample_100kB.docx
  [[ "$output" = 1 ]]

  #run grep -c 'Date' $BATS_TEST_DIRNAME/docxbox ls files/file-sample_100kB.docx
  #[[ "$output" = 1 ]]

  #run grep -c 'Time' $BATS_TEST_DIRNAME/docxbox ls files/file-sample_100kB.docx
  #[[ "$output" = 1 ]]

  #run grep -c 'Name' $BATS_TEST_DIRNAME/docxbox ls files/file-sample_100kB.docx
  #[[ "$output" = 1 ]]
}

@test "\"docxbox ls filename.docx\" outputs contained files" {
  run grep -c '_rels/.rels' $BATS_TEST_DIRNAME/docxbox ls files/file-sample_100kB.docx
  run grep -c 'word/settings.xml' $BATS_TEST_DIRNAME/docxbox ls files/file-sample_100kB.docx
  run grep -c 'word/_rels/document.xml.rels' $BATS_TEST_DIRNAME/docxbox ls files/file-sample_100kB.docx
  run grep -c 'word/fonttable.xml' $BATS_TEST_DIRNAME/docxbox ls files/file-sample_100kB.docx
}

@test "\"docxbox ls filename.docx\" outputs amount of contained files" {
  run grep -c '12 files' $BATS_TEST_DIRNAME/docxbox ls files/file-sample_100kB.docx
}

@test "\"docxbox ls filename.docx\" outputs files' date and time" {
  run grep -c '08/16/2017' $BATS_TEST_DIRNAME/docxbox ls files/file-sample_100kB.docx
  run grep -c '12:28' $BATS_TEST_DIRNAME/docxbox ls files/file-sample_100kB.docx
}
