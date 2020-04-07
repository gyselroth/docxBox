#!/usr/bin/env bats

load _helper

@test "Output of \"docxbox lsj filename.docx\" contains files' and directories' attributes" {
  run grep -c 'length' $BATS_TEST_DIRNAME/docxbox lsj files/file-sample_100kB.docx
  run grep -c 'date' $BATS_TEST_DIRNAME/docxbox lsj files/file-sample_100kB.docx
  run grep -c 'time' $BATS_TEST_DIRNAME/docxbox lsj files/file-sample_100kB.docx
  run grep -c 'name' $BATS_TEST_DIRNAME/docxbox lsj files/file-sample_100kB.docx
}

@test "Output of \"docxbox lsj filename.docx\" outputs contained files" {
  run grep -c '_rels/.rels' $BATS_TEST_DIRNAME/docxbox lsj files/file-sample_100kB.docx
  run grep -c 'word/settings.xml' $BATS_TEST_DIRNAME/docxbox lsj files/file-sample_100kB.docx
  run grep -c 'word/_rels/document.xml.rels' $BATS_TEST_DIRNAME/docxbox lsj files/file-sample_100kB.docx
  run grep -c 'word/fonttable.xml' $BATS_TEST_DIRNAME/docxbox lsj files/file-sample_100kB.docx
}

@test "Output of \"docxbox lsj filename.docx\" contains amount of contained files" {
  run grep -c '12 files' $BATS_TEST_DIRNAME/docxbox lsj files/file-sample_100kB.docx
}

@test "Output of \"docxbox lsj filename.docx\" contains files' date and time" {
  run grep -c '08/16/2017' $BATS_TEST_DIRNAME/docxbox lsj files/file-sample_100kB.docx
  run grep -c '12:28' $BATS_TEST_DIRNAME/docxbox lsj files/file-sample_100kB.docx
}
