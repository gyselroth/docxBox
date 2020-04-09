#!/usr/bin/env bats

load _helper

@test "Output of \"docxbox lsj filename.docx\" contains files' and directories' attributes" {
  run grep -c 'lengthXXX' $BATS_TEST_DIRNAME/docxbox lsj files/sample_100kB.docx
  run grep -c 'date' $BATS_TEST_DIRNAME/docxbox lsj files/sample_100kB.docx
  run grep -c 'time' $BATS_TEST_DIRNAME/docxbox lsj files/sample_100kB.docx
  run grep -c 'name' $BATS_TEST_DIRNAME/docxbox lsj files/sample_100kB.docx
}

@test "Output of \"docxbox lsj filename.docx\" are contained files" {
  run grep -c '_rels/.rels' $BATS_TEST_DIRNAME/docxbox lsj files/sample_100kB.docx
  run grep -c 'word/settings.xml' $BATS_TEST_DIRNAME/docxbox lsj files/sample_100kB.docx
  run grep -c 'word/_rels/document.xml.rels' $BATS_TEST_DIRNAME/docxbox lsj files/sample_100kB.docx
  run grep -c 'word/fonttable.xml' $BATS_TEST_DIRNAME/docxbox lsj files/sample_100kB.docx
}

@test "Output of \"docxbox lsj filename.docx\" contains amount of contained files" {
  run grep -c '12 files' $BATS_TEST_DIRNAME/docxbox lsj files/sample_100kB.docx
}

@test "Output of \"docxbox lsj filename.docx\" contains files' date and time" {
  run grep -c '08/16/2017' $BATS_TEST_DIRNAME/docxbox lsj files/sample_100kB.docx
  run grep -c '12:28' $BATS_TEST_DIRNAME/docxbox lsj files/sample_100kB.docx
}

@test "Output of \"docxbox ls {missing argument}\" is an error message" {
  run $BATS_TEST_DIRNAME/docxbox lsj
  [ "$status" -eq 125 ]
  [ "Missing argument: DOCX filename" = "${lines[0]}" ]
}
