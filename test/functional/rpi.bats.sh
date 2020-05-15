#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

docxbox=""$BATS_TEST_DIRNAME"/docxbox"
path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"
path_extracted_image="test/functional/tmp/unziped/word/media/image1.jpeg"

base_command="docxbox rpi filename.docx"

@test "Output of \"docxbox rpi {missing filename}\" is an error message" {
  run "${docxbox}" rpi
  [ "$status" -ne 0 ]
  [ "Missing argument: DOCX filename" = "${lines[0]}" ]
}

@test "Output of \"${base_command} {missing argument}\" is an error message" {
  run "${docxbox}" rpi "${path_docx}"
  [ "$status" -ne 0 ]
  [ "Missing argument: Filename of image to be replaced" = "${lines[0]}" ]
}

missing_argument="imageName {missingReplacementImageName}"
@test "Output of \"${base_command} ${missing_argument}\" is an error message" {
  run "${docxbox}" rpi "${path_docx}" image1.jpeg
  [ "$status" -ne 0 ]
  [ "Missing argument: Filename of replacement image" = "${lines[0]}" ]
}

appendix="an image can be replaced"
@test "With \"${base_command} imageName replacementImageName\" ${appendix}" {
  path_jpeg="test/files/images/2100x400.jpeg"

  run "${docxbox}" rpi "${path_docx}" image1.jpeg "${path_jpeg}"
  [ "$status" -eq 0 ]
  if [ ! -d test/functional/tmp/unziped ]; then
    mkdir test/functional/tmp/unziped;
    unzip "${path_docx}" -d test/functional/tmp/unziped;
  fi

  file "${path_extracted_image}" | grep --count "2100x400"
}

arguments="imageName replacementImageName newFilename.docx"
appendix_new_docx="an image can be replaced and saved to new doxc"
@test "With \"${base_command} ${arguments}\" ${appendix_new_docx}" {
  path_docx_out="test/functional/tmp/newImage.docx"
  path_jpeg="test/files/images/2100x400.jpeg"

  run "${docxbox}" rpi "${path_docx}" image1.jpeg "${path_jpeg}" "${path_docx_out}"
  [ "$status" -eq 0 ]
  if [ ! -d test/functional/tmp/unziped ]; then
    mkdir test/functional/tmp/unziped;
    unzip test/functional/tmp/newImage.docx -d test/functional/tmp/unziped;
  fi

  file "${path_extracted_image}" | grep --count "2100x400"
}
