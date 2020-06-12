#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

docxbox=""$BATS_TEST_DIRNAME"/docxbox"
path_docx="test/functional/tmp/cp_table_unordered_list_images.docx"
error="is an error message"
display_file="word/document.xml"

base_command="docxbox rpt filename.docx"

@test "Output of \"docxbox rpt {missing filename}\" ${error}" {
  run "${docxbox}" rpt
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: DOCX filename" = "${lines[0]}" ]
}

@test "Output of \"${base_command} {missing arguments}\" ${error}" {
  pattern="docxBox Error - Missing argument: String to be found (and replaced)"

  run "${docxbox}" rpt "${path_docx}"
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]
}

missing_argument="stringToBeReplaced {missing argument}"
@test "Output of \"${base_command} ${missing_argument}\" ${error}" {
  run "${docxbox}" rpt "${path_docx}" toBeReplaced
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: Replacement" = "${lines[0]}" ]
}

arguments="stringToBeReplaced replacementString"
appendix="the stringToBeReplaced gets replaced"
@test "With \"${base_command} ${arguments}\" ${appendix}" {
  run "${docxbox}" rpt "${path_docx}" Lorem Dorem
  [ "$status" -eq 0 ]
  "${docxbox}" txt "${path_docx}" | grep --count Dorem
}

arguments_new_docx="stringToBeReplaced replacementString newFile.docx"
appendix_new_docx="the stringToBeReplaced gets replaced and is saved to new file"
@test "With \"${base_command} ${arguments_new_docx}\" ${appendix_new_docx}" {
  path_docx_out="test/functional/tmp/replacedString.docx"

  run "${docxbox}" rpt "${path_docx}" Lorem Dorem "${path_docx_out}"
  [ "$status" -eq 0 ]
  "${docxbox}" txt "${path_docx_out}" | grep --count Dorem
}

@test "With \"${base_command} heading_as_JSON\" the given string is replaced by a h1" {
  heading="{\"h1\":{\"text\":\"Foo\"}}"

  "${docxbox}" cat "${path_docx}" "${display_file}" | grep --invert-match "<w:pStyle w:val=\"para1\"/>"

  run "${docxbox}" rpt "${path_docx}" Officia ${heading}
  [ "$status" -eq 0 ]

  "${docxbox}" cat "${path_docx}" "${display_file}" | grep --count "<w:pStyle w:val=\"para1\"/>"
  "${docxbox}" lsl "${path_docx}" "para1" | grep --count "${display_file}"
}

@test "With \"${base_command} heading_as_JSON\" the given string is replaced by a h2" {
  heading="{\"h2\":{\"text\":\"Foo\"}}"

  "${docxbox}" cat "${path_docx}" "${display_file}" | grep --invert-match "<w:pStyle w:val=\"para2\"/>"

  run "${docxbox}" rpt "${path_docx}" Officia ${heading}
  [ "$status" -eq 0 ]

  "${docxbox}" cat "${path_docx}" "${display_file}" | grep --count "<w:pStyle w:val=\"para2\"/>"
  "${docxbox}" lsl "${path_docx}" "para2" | grep --count "${display_file}"
}

@test "With \"${base_command} heading_as_JSON\" the given string is replaced by a h3" {
  heading="{\"h3\":{\"text\":\"Foo\"}}"

  "${docxbox}" cat "${path_docx}" "${display_file}" | grep --invert-match "<w:pStyle w:val=\"para3\"/>"

  run "${docxbox}" rpt "${path_docx}" Officia ${heading}
  [ "$status" -eq 0 ]

  "${docxbox}" cat "${path_docx}" "${display_file}" | grep --count "<w:pStyle w:val=\"para3\"/>"
  "${docxbox}" lsl "${path_docx}" "para3" | grep --count "${display_file}"
}

@test "With \"${base_command} ul_as_JSON\" the given string is replaced by a unordered list" {
  list="{\"ul\":{\"items\":[\"item-1\",\"item-2\",\"item-3\"]}}"
  path_docx="test/functional/tmp/cp_bio_assay.docx"

  "${docxbox}" cat "${path_docx}" "${display_file}" | grep --invert-match "<w:numId w:val=\"1\"/>"

  run "${docxbox}" rpt "${path_docx}" 5NISINisi ${list}
  [ "$status" -eq 0 ]

  "${docxbox}" cat "${path_docx}" "${display_file}" | grep --count "<w:numId w:val=\"1\"/>"
  "${docxbox}" lsl "${path_docx}" "w:numPr" | grep --count "${display_file}"
}

# ordered list can not be inserted atm, so no assertions can be made
#@test "With \"${base_command} ol_as_JSON\" the given string is replaced by a ordered list" {
#  list="{\"ol\":{\"items\":[\"item-1\",\"item-2\",\"item-3\"]}}"
#  path_docx="test/functional/tmp/cp_bio_assay.docx"
#
#  "${docxbox}" cat "${path_docx}" "${display_file}" | grep --invert-match "<w:numId w:val=\"2\"/>"
#
#  run "${docxbox}" rpt "${path_docx}" 5NISINisi ${list}
#  [ "$status" -eq 0 ]
#
#  "${docxbox}" cat "${path_docx}" "${display_file}" | grep --count "<w:numId w:val=\"2\"/>"
#  "${docxbox}" lsl "${path_docx}" "w:numPr" | grep --count "${display_file}"
#}

@test "With \"${base_command} hyperlink_as_JSON\" the given string is replaced by a hyperlink" {
  url="\"url\":\"https://github.com/gyselroth/docxbox\""
  link="{\"link\":{\"text\":\"docxBox\",${url}}}"

  "${docxbox}" cat "${path_docx}" "${display_file}" | grep --invert-match "</w:hyperlink>"

  run "${docxbox}" rpt "${path_docx}" Officia ${link}
  [ "$status" -eq 0 ]

  "${docxbox}" cat "${path_docx}" "${display_file}" | grep --count "</w:hyperlink>"
  "${docxbox}" lsl "${path_docx}" "</w:hyperlink>" | grep --count "${display_file}"
}

image_replacement="the given string is replaced by an image"
@test "With \"${base_command} image_as_JSON\" ${image_replacement} using EMU" {
  image="{\"image\":{\"size\":[2438400,1828800]}}"
  image_path="test/files/images/2100x400.jpeg"

  "${docxbox}" cat "${path_docx}" "${display_file}" | grep --invert-match "<w:drawing>"
  "${docxbox}" cat "${path_docx}" "${display_file}" | grep --invert-match "</w:drawing>"

  run "${docxbox}" rpt "${path_docx}" Officia ${image} ${image_path}
  [ "$status" -eq 0 ]

  "${docxbox}" cat "${path_docx}" "${display_file}" | grep --count "<w:drawing>"
  "${docxbox}" cat "${path_docx}" "${display_file}" | grep --count "</w:drawing>"
  "${docxbox}" lsl "${path_docx}" "<w:drawing>" | grep --count "${display_file}"
  "${docxbox}" lsl "${path_docx}" "</w:drawing>" | grep --count "${display_file}"
}

@test "With \"${base_command} image_as_JSON\" ${image_replacement} using pixels" {
  image="{\"image\":{\"size\":[\"2100px\",\"400px\"]}}"
  image_path="test/files/images/2100x400.jpeg"

  "${docxbox}" cat "${path_docx}" "${display_file}" | grep --invert-match "<w:drawing>"
  "${docxbox}" cat "${path_docx}" "${display_file}" | grep --invert-match "</w:drawing>"

  run "${docxbox}" rpt "${path_docx}" Officia ${image} ${image_path}
  [ "$status" -eq 0 ]

  "${docxbox}" cat "${path_docx}" "${display_file}" | grep --count "<w:drawing>"
  "${docxbox}" cat "${path_docx}" "${display_file}" | grep --count "</w:drawing>"
  "${docxbox}" lsl "${path_docx}" "<w:drawing>" | grep --count "${display_file}"
  "${docxbox}" lsl "${path_docx}" "</w:drawing>" | grep --count "${display_file}"
}

@test "With \"${base_command} table_as_JSON\" the given string is replaced by a table" {
  header="\"header\":[\"A\",\"B\",\"C\"]"
  first_column="[\"a1\",\"a2\",\"a3\"]"
  second_column="[\"b1\",\"b2\",\"b3\"]"
  third_column="[\"c1\",\"c2\",\"c3\"]"
  content="\"content\":[${first_column},${second_column},${third_column}]"
  table="{\"table\":{${header},${content}}}"

  pattern="<w:tblStyle w:val=\"TableGrid\"/>"

  "${docxbox}" cat "${path_docx}" "${display_file}" | grep --invert-match "${pattern}"

  run "${docxbox}" rpt "${path_docx}" Officia ${table}
  [ "$status" -eq 0 ]

  "${docxbox}" cat "${path_docx}" "${display_file}" | grep --count "${pattern}"
  "${docxbox}" lsl "${path_docx}" "<w:tbl>" | grep --count "${display_file}"
}

@test "Output of \"docxbox rpt ${arguments} nonexistent.docx\" is an error message" {
  err_log="test/functional/tmp/err.log"

  run "${docxbox}" rpt nonexistent.docx
  [ "$status" -ne 0 ]

  "${docxbox}" rpt nonexistent.docx Lorem Dorem 2>&1 | tee "${err_log}"
  cat "${err_log}" | grep --count "docxBox Error - File not found:"
}

@test "Output of \"docxbox rpt ${arguments} wrong_file_type\" ${error}" {
  pattern="docxBox Error - File is no ZIP archive:"
  err_log="test/functional/tmp/err.log"
  wrong_file_types=(
  "test/functional/tmp/cp_lorem_ipsum.pdf"
  "test/functional/tmp/cp_mock_csv.csv"
  "test/functional/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    "${docxbox}" rpt "${i}" Lorem Dorem 2>&1 | tee "${err_log}"
    cat "${err_log}" | grep --count "${pattern}"
  done
}
