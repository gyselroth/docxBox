#!/usr/bin/env bats

load _helper

@test "Output of \"docxbox rpi {missing filename}\" is an error message" {
  run "$BATS_TEST_DIRNAME"/docxbox rpi
  [ "$status" -ne 0 ]
  [ "Missing argument: Filename of DOCX" = "${lines[0]}" ]
}

@test "Output of \"docxbox rpi filename.docx {missing argument}\" is an error message" {
  run $BATS_TEST_DIRNAME/docxbox rpi test/files/docx/cp_table_unordered_list_images.docx
  [ "$status" -ne 0 ]
  [ "Missing argument: Filename of image to be replaced" = "${lines[0]}" ]
}

@test "Output of \"docxbox rpi filename.docx imageName {missingReplacementImageName}\" is an error message" {
  run $BATS_TEST_DIRNAME/docxbox rpi test/files/docx/cp_table_unordered_list_images.docx image1.jpeg
  [ "$status" -ne 0 ]
  [ "Missing argument: Filename of replacement image" = "${lines[0]}" ]
}

@test "With \"docxbox rpi filename.docx imageName replacementImageName\" an image can be replaced" {
  run $BATS_TEST_DIRNAME/docxbox rpi test/files/docx/cp_table_unordered_list_images.docx image1.jpeg test/files/images/2100x400.jpeg
  [ "$status" -eq 0 ]
  if [ ! -d test/files/docx/unziped ]; then
    mkdir test/files/docx/unziped;
    unzip test/files/docx/cp_table_unordered_list_images.docx -d test/files/docx/unziped;
  fi

  identify -verbose test/files/docx/unziped/word/media/image1.jpeg | grep -c "Resolution: 600x600"

  if [ -d test/files/docx/unziped ]; then
    rm -r  test/files/docx/unziped;
  fi
}

@test "With \"docxbox rpi filename.docx imageName replacementImageName newFilename.docx\" an image can be replaced and saved to new doxc" {
  run $BATS_TEST_DIRNAME/docxbox rpi test/files/docx/cp_table_unordered_list_images.docx image1.jpeg test/files/images/2100x400.jpeg test/files/docx/newImage.docx
  [ "$status" -eq 0 ]
  if [ ! -d test/files/docx/unziped ]; then
    mkdir test/files/docx/unziped;
    unzip test/files/docx/newImage.docx -d test/files/docx/unziped;
  fi

  identify -verbose test/files/docx/unziped/word/media/image1.jpeg | grep -c "Resolution: 600x600"

  if [ -d test/files/docx/unziped ]; then
    rm -r  test/files/docx/unziped;
  fi

  if [ -f test/files/docx/newImage.docx ]; then
    rm test/files/docx/newImage.docx;
  fi
}
