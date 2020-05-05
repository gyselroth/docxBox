#!/usr/bin/env bats

load _helper

@test "Output of \"docxbox rpi {missing filename}\" is an error message" {
  run "$BATS_TEST_DIRNAME"/docxbox rpi
  [ "$status" -ne 0 ]
  [ "Missing argument: DOCX filename" = "${lines[0]}" ]
}

@test "Output of \"docxbox rpi filename.docx {missing argument}\" is an error message" {
  run "$BATS_TEST_DIRNAME"/docxbox rpi test/functional/tmp/cp_table_unordered_list_images.docx
  [ "$status" -ne 0 ]
  [ "Missing argument: Filename of image to be replaced" = "${lines[0]}" ]
}

@test "Output of \"docxbox rpi filename.docx imageName {missingReplacementImageName}\" is an error message" {
  run "$BATS_TEST_DIRNAME"/docxbox rpi test/functional/tmp/cp_table_unordered_list_images.docx image1.jpeg
  [ "$status" -ne 0 ]
  [ "Missing argument: Filename of replacement image" = "${lines[0]}" ]
}

@test "With \"docxbox rpi filename.docx imageName replacementImageName\" an image can be replaced" {
  run "$BATS_TEST_DIRNAME"/docxbox rpi test/functional/tmp/cp_table_unordered_list_images.docx image1.jpeg test/files/images/2100x400.jpeg
  [ "$status" -eq 0 ]
    if [ ! -d test/functional/tmp/unziped ]; then
    mkdir test/functional/tmp/unziped;
    unzip test/functional/tmp/cp_table_unordered_list_images.docx -d test/functional/tmp/unziped;
  fi

  file test/functional/tmp/unziped/word/media/image1.jpeg | grep -c "2100x400"
}

@test "With \"docxbox rpi filename.docx imageName replacementImageName newFilename.docx\" an image can be replaced and saved to new doxc" {
  run "$BATS_TEST_DIRNAME"/docxbox rpi test/functional/tmp/cp_table_unordered_list_images.docx image1.jpeg test/files/images/2100x400.jpeg test/functional/tmp/newImage.docx
  [ "$status" -eq 0 ]
  if [ ! -d test/functional/tmp/unziped ]; then
    mkdir test/functional/tmp/unziped;
    unzip test/functional/tmp/newImage.docx -d test/functional/tmp/unziped;
  fi

  file test/functional/tmp/unziped/word/media/image1.jpeg | grep -c "2100x400"
}
