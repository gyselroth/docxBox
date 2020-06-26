#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

VALGRIND="valgrind -v --leak-check=full\
 --log-file=test/assets/documents/other/mem-leak.log"

if $IS_VALGRIND_TEST;
then
  DOCXBOX_BINARY="${VALGRIND} $BATS_TEST_DIRNAME/../tmp/docxbox"
else
  DOCXBOX_BINARY="$BATS_TEST_DIRNAME/../tmp/docxbox"
fi

path_docx="test/tmp/cp_table_unordered_list_images.docx"
error="is an error message"
display_file="word/document.xml"

base_command="docxbox rpt filename.docx"

@test "Output of \"docxbox rpt {missing filename}\" ${error}" {
  run ${DOCXBOX_BINARY} rpt
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: DOCX filename" = "${lines[0]}" ]
}

@test "Output of \"${base_command} {missing arguments}\" ${error}" {
  pattern="docxBox Error - Missing argument: String to be found (and replaced)"

  run ${DOCXBOX_BINARY} rpt "${path_docx}"
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]
}

missing_argument="stringToBeReplaced {missing argument}"
@test "Output of \"${base_command} ${missing_argument}\" ${error}" {
  run ${DOCXBOX_BINARY} rpt "${path_docx}" toBeReplaced
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: Replacement" = "${lines[0]}" ]
}

arguments="stringToBeReplaced replacementString"
appendix="the stringToBeReplaced gets replaced"
@test "With \"${base_command} ${arguments}\" ${appendix}" {
  run ${DOCXBOX_BINARY} rpt "${path_docx}" Lorem Dorem
  [ "$status" -eq 0 ]
  ${DOCXBOX_BINARY} txt "${path_docx}" | grep --count Dorem
}

arguments_new_docx="stringToBeReplaced replacementString newFile.docx"
appendix_new_docx="the stringToBeReplaced gets replaced and is saved to new file"
@test "With \"${base_command} ${arguments_new_docx}\" ${appendix_new_docx}" {
  path_docx_out="test/tmp/replacedString.docx"

  run ${DOCXBOX_BINARY} rpt "${path_docx}" Lorem Dorem "${path_docx_out}"
  [ "$status" -eq 0 ]
  ${DOCXBOX_BINARY} txt "${path_docx_out}" | grep --count Dorem
}

@test "With \"${base_command} heading_as_JSON\" the given string is replaced by a h1" {
  heading="{\"h1\":{\"text\":\"Foo\"}}"

  ${DOCXBOX_BINARY} cat "${path_docx}" "${display_file}" | grep --invert-match "<w:pStyle w:val=\"para1\"/>"

  run ${DOCXBOX_BINARY} rpt "${path_docx}" Officia ${heading}
  [ "$status" -eq 0 ]

  ${DOCXBOX_BINARY} cat "${path_docx}" "${display_file}" | grep --count "<w:pStyle w:val=\"para1\"/>"
  ${DOCXBOX_BINARY} lsl "${path_docx}" "para1" | grep --count "${display_file}"
}

@test "With \"${base_command} heading_as_JSON\" the given string is replaced by a h2" {
  heading="{\"h2\":{\"text\":\"Foo\"}}"

  ${DOCXBOX_BINARY} cat "${path_docx}" "${display_file}" | grep --invert-match "<w:pStyle w:val=\"para2\"/>"

  run ${DOCXBOX_BINARY} rpt "${path_docx}" Officia ${heading}
  [ "$status" -eq 0 ]

  ${DOCXBOX_BINARY} cat "${path_docx}" "${display_file}" | grep --count "<w:pStyle w:val=\"para2\"/>"
  ${DOCXBOX_BINARY} lsl "${path_docx}" "para2" | grep --count "${display_file}"
}

@test "With \"${base_command} heading_as_JSON\" the given string is replaced by a h3" {
  heading="{\"h3\":{\"text\":\"Foo\"}}"

  ${DOCXBOX_BINARY} cat "${path_docx}" "${display_file}" | grep --invert-match "<w:pStyle w:val=\"para3\"/>"

  run ${DOCXBOX_BINARY} rpt "${path_docx}" Officia ${heading}
  [ "$status" -eq 0 ]

  ${DOCXBOX_BINARY} cat "${path_docx}" "${display_file}" | grep --count "<w:pStyle w:val=\"para3\"/>"
  ${DOCXBOX_BINARY} lsl "${path_docx}" "para3" | grep --count "${display_file}"
}

@test "With \"${base_command} ul_as_JSON\" the given string is replaced by a unordered list" {
  list="{\"ul\":{\"items\":[\"item-1\",\"item-2\",\"item-3\"]}}"
  path_docx="test/tmp/cp_bio_assay.docx"

  ${DOCXBOX_BINARY} cat "${path_docx}" "${display_file}" | grep --invert-match "<w:numId w:val=\"1\"/>"

  run ${DOCXBOX_BINARY} rpt "${path_docx}" 5NISINisi ${list}
  [ "$status" -eq 0 ]

  ${DOCXBOX_BINARY} cat "${path_docx}" "${display_file}" | grep --count "<w:numId w:val=\"1\"/>"
  ${DOCXBOX_BINARY} lsl "${path_docx}" "w:numPr" | grep --count "${display_file}"
}

@test "With \"${base_command} ol_as_JSON\" the given string is replaced by a ordered list" {
  list="{\"ol\":{\"items\":[\"item-1\",\"item-2\",\"item-3\"]}}"
  path_docx="test/tmp/cp_bio_assay.docx"

  ${DOCXBOX_BINARY} cat "${path_docx}" "${display_file}" | grep --invert-match "<w:numId w:val=\"2\"/>"

  run ${DOCXBOX_BINARY} rpt "${path_docx}" 5NISINisi ${list}
  [ "$status" -eq 0 ]

  ${DOCXBOX_BINARY} cat "${path_docx}" "${display_file}" | grep --count "<w:numId w:val=\"2\"/>"
  ${DOCXBOX_BINARY} lsl "${path_docx}" "w:numPr" | grep --count -q "${display_file}"
}

@test "With \"${base_command} hyperlink_as_JSON\" the given string is replaced by a hyperlink" {
  url="\"url\":\"https://github.com/gyselroth/docxbox\""
  link="{\"link\":{\"text\":\"docxBox\",${url}}}"

  ${DOCXBOX_BINARY} cat "${path_docx}" "${display_file}" | grep --invert-match "</w:hyperlink>"

  run ${DOCXBOX_BINARY} rpt "${path_docx}" Officia ${link}
  [ "$status" -eq 0 ]

  ${DOCXBOX_BINARY} cat "${path_docx}" "${display_file}" | grep --count "</w:hyperlink>"
  ${DOCXBOX_BINARY} lsl "${path_docx}" "</w:hyperlink>" | grep --count "${display_file}"
}

image_replacement="the given string is replaced by an image"
@test "With \"${base_command} image_as_JSON\" ${image_replacement} using EMU" {
  image="{\"image\":{\"size\":[2438400,1828800]}}"
  image_path="test/assets/images/2100x400.jpeg"

  ${DOCXBOX_BINARY} cat "${path_docx}" "${display_file}" | grep --invert-match "<w:drawing>"
  ${DOCXBOX_BINARY} cat "${path_docx}" "${display_file}" | grep --invert-match "</w:drawing>"

  run ${DOCXBOX_BINARY} rpt "${path_docx}" Officia ${image} ${image_path}
  [ "$status" -eq 0 ]

  ${DOCXBOX_BINARY} cat "${path_docx}" "${display_file}" | grep --count "<w:drawing>"
  ${DOCXBOX_BINARY} cat "${path_docx}" "${display_file}" | grep --count "</w:drawing>"
  ${DOCXBOX_BINARY} lsl "${path_docx}" "<w:drawing>" | grep --count "${display_file}"
  ${DOCXBOX_BINARY} lsl "${path_docx}" "</w:drawing>" | grep --count "${display_file}"
}

@test "With \"${base_command} image_as_JSON\" ${image_replacement} using pixels" {
  image="{\"image\":{\"size\":[\"2100px\",\"400px\"]}}"
  image_path="test/assets/images/2100x400.jpeg"

  ${DOCXBOX_BINARY} cat "${path_docx}" "${display_file}" | grep --invert-match "<w:drawing>"
  ${DOCXBOX_BINARY} cat "${path_docx}" "${display_file}" | grep --invert-match "</w:drawing>"

  run ${DOCXBOX_BINARY} rpt "${path_docx}" Officia ${image} ${image_path}
  [ "$status" -eq 0 ]

  ${DOCXBOX_BINARY} cat "${path_docx}" "${display_file}" | grep --count "<w:drawing>"
  ${DOCXBOX_BINARY} cat "${path_docx}" "${display_file}" | grep --count "</w:drawing>"
  ${DOCXBOX_BINARY} lsl "${path_docx}" "<w:drawing>" | grep --count "${display_file}"
  ${DOCXBOX_BINARY} lsl "${path_docx}" "</w:drawing>" | grep --count "${display_file}"
}

@test "With \"${base_command} table_as_JSON\" the given string is replaced by a table" {
  header="\"header\":[\"header_one\",\"B\",\"C\"]"
  first_column="[\"a1\",\"a2\",\"a3\"]"
  second_column="[\"b1\",\"b2\",\"b3\"]"
  third_column="[\"c1\",\"c2\",\"c3\"]"
  content="\"content\":[${first_column},${second_column},${third_column}]"
  table="{\"table\":{${header},${content}}}"

  pattern="<w:tblStyle w:val=\"TableGrid\"/>"

  ${DOCXBOX_BINARY} cat "${path_docx}" "${display_file}" | grep --invert-match "${pattern}"

  run ${DOCXBOX_BINARY} rpt "${path_docx}" Officia ${table}
  [ "$status" -eq 0 ]

  ${DOCXBOX_BINARY} cat "${path_docx}" "${display_file}" | grep --count "${pattern}"
  ${DOCXBOX_BINARY} lsl "${path_docx}" "<w:tbl>" | grep --count "${display_file}"
}

@test "With \"${base_command} table_as_JSON\" the given string is replaced by a table with header" {
  header="\"header\":[\"header_one\",\"B\",\"C\"]"
  first_column="[\"a1\",\"a2\",\"a3\"]"
  second_column="[\"b1\",\"b2\",\"b3\"]"
  third_column="[\"c1\",\"c2\",\"c3\"]"
  content="\"content\":[${first_column},${second_column},${third_column}]"
  table="{\"table\":{${header},${content}}}"

  pattern="<w:tblStyle w:val=\"TableGrid\"/>"

  ${DOCXBOX_BINARY} cat "${path_docx}" "${display_file}" | grep --invert-match "${pattern}"

  run ${DOCXBOX_BINARY} rpt "${path_docx}" Officia ${table}
  [ "$status" -eq 0 ]

  ${DOCXBOX_BINARY} lsl "${path_docx}" "header_one" | grep --count -q "${display_file}"
  ${DOCXBOX_BINARY} cat "${path_docx}" "${display_file}" | grep --count -q "header_one"
}

@test "Output of \"docxbox rpt ${arguments} nonexistent.docx\" is an error message" {
  err_log="test/tmp/err.log"

  run ${DOCXBOX_BINARY} rpt nonexistent.docx
  [ "$status" -ne 0 ]

  ${DOCXBOX_BINARY} rpt nonexistent.docx Lorem Dorem 2>&1 | tee "${err_log}"
  cat "${err_log}" | grep --count "docxBox Error - File not found:"
}

@test "Output of \"docxbox rpt ${arguments} wrong_file_type\" ${error}" {
  pattern="docxBox Error - File is no ZIP archive:"
  err_log="test/tmp/err.log"
  wrong_file_types=(
  "test/tmp/cp_lorem_ipsum.pdf"
  "test/tmp/cp_mock_csv.csv"
  "test/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    ${DOCXBOX_BINARY} rpt "${i}" Lorem Dorem 2>&1 | tee "${err_log}"
    cat "${err_log}" | grep --count "${pattern}"
  done
}
