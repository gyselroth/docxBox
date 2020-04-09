#!/usr/bin/env bats

load _helper

@test "Output of \"docxbox ls filename.docx\" contains files' and directories' attributes" {
  run $BATS_TEST_DIRNAME/docxbox ls test/files/sample_100kB.docx
  [ "$status" -eq 0 ]
  [ "  Length      Date    Time    Name" = "${lines[0]}" ]
}

@test "Output of \"docxbox ls filename.docx\" are contained files" {
  run $BATS_TEST_DIRNAME/docxbox ls test/files/sample_100kB.docx
  [ "$status" -eq 0 ]
  [ "      272  04/07/2020 20:14   docProps/app.xml" = "${lines[3]}" ]
#  run grep -c '_rels/.rels' $BATS_TEST_DIRNAME/docxbox ls test/files/sample_100kB.docx
#  run grep -c 'word/settings.xml' $BATS_TEST_DIRNAME/docxbox ls files/sample_100kB.docx
#  run grep -c 'word/_rels/document.xml.rels' $BATS_TEST_DIRNAME/docxbox ls files/sample_100kB.docx
#  run grep -c 'word/fonttable.xml' $BATS_TEST_DIRNAME/docxbox ls files/sample_100kB.docx
}

@test "Output of \"docxbox ls filename.docx\" contains amount of contained files" {
  run grep -c '1000 files' $BATS_TEST_DIRNAME/docxbox ls files/sample_100kB.docx
#  [ "1" = "${lines[0]}" ]
}

@test "Output of \"docxbox ls filename.docx\" contains files' date and time" {
  run grep -c '08/16/2017' $BATS_TEST_DIRNAME/docxbox ls files/sample_100kB.docx
  run grep -c '12:28' $BATS_TEST_DIRNAME/docxbox ls files/sample_100kB.docx
}

@test "Output of \"docxbox ls {missing argument}\" is an error message" {
  run $BATS_TEST_DIRNAME/docxbox ls
  [ "$status" -eq 125 ]
  [ "Missing argument: DOCX filename" = "${lines[0]}" ]
}
