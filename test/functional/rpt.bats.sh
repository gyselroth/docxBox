#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper
source ./test/functional/_set_docxbox_binary.sh

CMD="docxbox rpt"

PATH_DOCX="test/tmp/cp_table_unordered_list_images.docx"
PATH_DOCX_NEW="test/tmp/cp_plain_text.docx"
ERR_LOG="test/tmp/err.log"

DISPLAY_FILE="word/document.xml"

@test "${BATS_TEST_NUMBER}: \"${CMD} {missing filename}\" prints an error message" {
  run ${DOCXBOX_BINARY} rpt
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: DOCX filename" = "${lines[0]}" ]

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx {missing arguments}\" prints an error message" {
  local pattern="docxBox Error - Missing argument: String to be found (and replaced)"

  run ${DOCXBOX_BINARY} rpt "${PATH_DOCX}"
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx stringToBeReplaced {missing argument}\" prints an error message" {
  run ${DOCXBOX_BINARY} rpt "${PATH_DOCX}" toBeReplaced
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: Replacement" = "${lines[0]}" ]

  source ./test/functional/_check_for_valgrind_errors.sh
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx stringToBeReplaced replacementString\" replaces stringToBeReplaced" {
  run ${DOCXBOX_BINARY} rpt "${PATH_DOCX}" Lorem Dorem
  [ "$status" -eq 0 ]

  source ./test/functional/_check_for_valgrind_errors.sh

  ${DOCXBOX_BINARY} txt "${PATH_DOCX}" | grep --count Dorem
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx stringToBeReplaced replacementString newFile.docx\" replaces stringToBeReplaced saves to new file" {
  local path_docx_out="test/tmp/replacedString.docx"

  run ${DOCXBOX_BINARY} rpt "${PATH_DOCX}" Lorem Dorem "${path_docx_out}"
  [ "$status" -eq 0 ]

  source ./test/functional/_check_for_valgrind_errors.sh

  ${DOCXBOX_BINARY} txt "${path_docx_out}" | grep --count Dorem
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx heading_as_JSON\" replaces given string with a h1" {
  local heading="{\"h1\":{\"text\":\"Foo\"}}"

  ${DOCXBOX_BINARY} cat "${PATH_DOCX}" "${DISPLAY_FILE}" | grep --invert-match "<w:pStyle w:val=\"para1\"/>"

  run ${DOCXBOX_BINARY} rpt "${PATH_DOCX}" Officia ${heading}
  [ "$status" -eq 0 ]

  source ./test/functional/_check_for_valgrind_errors.sh

  ${DOCXBOX_BINARY} cat "${PATH_DOCX}" "${DISPLAY_FILE}" | grep --count "<w:pStyle w:val=\"para1\"/>"
  ${DOCXBOX_BINARY} lsl "${PATH_DOCX}" "para1" | grep --count "${DISPLAY_FILE}"
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx heading_as_JSON\" replaces given string with a h2" {
  local heading="{\"h2\":{\"text\":\"Foo\"}}"

  ${DOCXBOX_BINARY} cat "${PATH_DOCX}" "${DISPLAY_FILE}" | grep --invert-match "<w:pStyle w:val=\"para2\"/>"

  run ${DOCXBOX_BINARY} rpt "${PATH_DOCX}" Officia ${heading}
  [ "$status" -eq 0 ]

  source ./test/functional/_check_for_valgrind_errors.sh

  ${DOCXBOX_BINARY} cat "${PATH_DOCX}" "${DISPLAY_FILE}" | grep --count "<w:pStyle w:val=\"para2\"/>"
  ${DOCXBOX_BINARY} lsl "${PATH_DOCX}" "para2" | grep --count "${DISPLAY_FILE}"
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx heading_as_JSON\" replaces given string with a h3" {
  local heading="{\"h3\":{\"text\":\"Foo\"}}"

  ${DOCXBOX_BINARY} cat "${PATH_DOCX}" "${DISPLAY_FILE}" | grep --invert-match "<w:pStyle w:val=\"para3\"/>"

  run ${DOCXBOX_BINARY} rpt "${PATH_DOCX}" Officia ${heading}
  [ "$status" -eq 0 ]

  source ./test/functional/_check_for_valgrind_errors.sh

  ${DOCXBOX_BINARY} cat "${PATH_DOCX}" "${DISPLAY_FILE}" | grep --count "<w:pStyle w:val=\"para3\"/>"
  ${DOCXBOX_BINARY} lsl "${PATH_DOCX}" "para3" | grep --count "${DISPLAY_FILE}"
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx ul_as_JSON\" replaces given string with an unordered list" {
  local list="{\"ul\":{\"items\":[\"item-1\",\"item-2\",\"item-3\"]}}"

  ${DOCXBOX_BINARY} cat "${PATH_DOCX_NEW}" "${DISPLAY_FILE}" | grep --invert-match "<w:numId w:val=\"1\"/>"

  run ${DOCXBOX_BINARY} rpt "${PATH_DOCX_NEW}" TITLE ${list}
  [ "$status" -eq 0 ]

  source ./test/functional/_check_for_valgrind_errors.sh

  ${DOCXBOX_BINARY} cat "${PATH_DOCX_NEW}" "${DISPLAY_FILE}" | grep --count "<w:numId w:val=\"1\"/>"
  ${DOCXBOX_BINARY} lsl "${PATH_DOCX_NEW}" "w:numPr" | grep --count "${DISPLAY_FILE}"
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx hyperlink_as_JSON\" replaces given string with a hyperlink" {
  local url="\"url\":\"https://github.com/gyselroth/docxbox\""
  local link="{\"link\":{\"text\":\"docxBox\",${url}}}"

  ${DOCXBOX_BINARY} cat "${PATH_DOCX}" "${DISPLAY_FILE}" | grep --invert-match "</w:hyperlink>"

  run ${DOCXBOX_BINARY} rpt "${PATH_DOCX}" Officia ${link}
  [ "$status" -eq 0 ]

  source ./test/functional/_check_for_valgrind_errors.sh

  ${DOCXBOX_BINARY} cat "${PATH_DOCX}" "${DISPLAY_FILE}" | grep --count "</w:hyperlink>"
  ${DOCXBOX_BINARY} lsl "${PATH_DOCX}" "</w:hyperlink>" | grep --count "${DISPLAY_FILE}"
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx image_as_JSON\" replaces given string with an image using EMU" {
  local image="{\"image\":{\"size\":[2438400,1828800]}}"
  local image_path="test/assets/images/2100x400.jpeg"

  ${DOCXBOX_BINARY} cat "${PATH_DOCX}" "${DISPLAY_FILE}" | grep --invert-match "<w:drawing>"
  ${DOCXBOX_BINARY} cat "${PATH_DOCX}" "${DISPLAY_FILE}" | grep --invert-match "</w:drawing>"

  run ${DOCXBOX_BINARY} rpt "${PATH_DOCX}" Officia ${image} ${image_path}
  [ "$status" -eq 0 ]

  source ./test/functional/_check_for_valgrind_errors.sh

  ${DOCXBOX_BINARY} cat "${PATH_DOCX}" "${DISPLAY_FILE}" | grep --count "<w:drawing>"
  ${DOCXBOX_BINARY} cat "${PATH_DOCX}" "${DISPLAY_FILE}" | grep --count "</w:drawing>"
  ${DOCXBOX_BINARY} lsl "${PATH_DOCX}" "<w:drawing>" | grep --count "${DISPLAY_FILE}"
  ${DOCXBOX_BINARY} lsl "${PATH_DOCX}" "</w:drawing>" | grep --count "${DISPLAY_FILE}"
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx image_as_JSON\" replaces given string with an image using pixels" {
  local image="{\"image\":{\"size\":[\"2100px\",\"400px\"]}}"
  local image_path="test/assets/images/2100x400.jpeg"

  ${DOCXBOX_BINARY} cat "${PATH_DOCX}" "${DISPLAY_FILE}" | grep --invert-match "<w:drawing>"
  ${DOCXBOX_BINARY} cat "${PATH_DOCX}" "${DISPLAY_FILE}" | grep --invert-match "</w:drawing>"

  run ${DOCXBOX_BINARY} rpt "${PATH_DOCX}" Officia ${image} ${image_path}
  [ "$status" -eq 0 ]

  source ./test/functional/_check_for_valgrind_errors.sh

  ${DOCXBOX_BINARY} cat "${PATH_DOCX}" "${DISPLAY_FILE}" | grep --count "<w:drawing>"
  ${DOCXBOX_BINARY} cat "${PATH_DOCX}" "${DISPLAY_FILE}" | grep --count "</w:drawing>"
  ${DOCXBOX_BINARY} lsl "${PATH_DOCX}" "<w:drawing>" | grep --count "${DISPLAY_FILE}"
  ${DOCXBOX_BINARY} lsl "${PATH_DOCX}" "</w:drawing>" | grep --count "${DISPLAY_FILE}"
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx table_as_JSON\" replaces given string with a table" {
  local header="\"header\":[\"header_one\",\"B\",\"C\"]"
  local first_column="[\"a1\",\"a2\",\"a3\"]"
  local second_column="[\"b1\",\"b2\",\"b3\"]"
  local third_column="[\"c1\",\"c2\",\"c3\"]"
  local content="\"content\":[${first_column},${second_column},${third_column}]"
  local table="{\"table\":{${header},${content}}}"

  local pattern="<w:tblStyle w:val=\"TableGrid\"/>"

  ${DOCXBOX_BINARY} cat "${PATH_DOCX}" "${DISPLAY_FILE}" | grep --invert-match "${pattern}"

  run ${DOCXBOX_BINARY} rpt "${PATH_DOCX}" Officia ${table}
  [ "$status" -eq 0 ]

  source ./test/functional/_check_for_valgrind_errors.sh

  ${DOCXBOX_BINARY} cat "${PATH_DOCX}" "${DISPLAY_FILE}" | grep --count "${pattern}"
  ${DOCXBOX_BINARY} lsl "${PATH_DOCX}" "<w:tbl>" | grep --count "${DISPLAY_FILE}"
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx table_as_JSON\" replaces given string with a table with header" {
  local header="\"header\":[\"header_one\",\"B\",\"C\"]"
  local first_column="[\"a1\",\"a2\",\"a3\"]"
  local second_column="[\"b1\",\"b2\",\"b3\"]"
  local third_column="[\"c1\",\"c2\",\"c3\"]"
  local content="\"content\":[${first_column},${second_column},${third_column}]"
  local table="{\"table\":{${header},${content}}}"

  local pattern="<w:tblStyle w:val=\"TableGrid\"/>"

  ${DOCXBOX_BINARY} cat "${PATH_DOCX}" "${DISPLAY_FILE}" | grep --invert-match "${pattern}"

  run ${DOCXBOX_BINARY} rpt "${PATH_DOCX}" Officia ${table}
  [ "$status" -eq 0 ]

  source ./test/functional/_check_for_valgrind_errors.sh

  ${DOCXBOX_BINARY} lsl "${PATH_DOCX}" "header_one" | grep --count -q "${DISPLAY_FILE}"
  ${DOCXBOX_BINARY} cat "${PATH_DOCX}" "${DISPLAY_FILE}" | grep --count -q "header_one"
}

@test "${BATS_TEST_NUMBER}: \"${CMD} filename.docx lower_case_string\" replaces given string case sensitive" {
  run ${DOCXBOX_BINARY} rpt "${PATH_DOCX_NEW}" "text" "FooBar"
  [ "$status" -eq 0 ]

  source ./test/functional/_check_for_valgrind_errors.sh

  ${DOCXBOX_BINARY} txt "${PATH_DOCX_NEW}" | grep --count "Text"
  ${DOCXBOX_BINARY} txt "${PATH_DOCX_NEW}" | grep --count "FooBar"
  ${DOCXBOX_BINARY} txt "${PATH_DOCX_NEW}" | grep --count --invert-match "text"
}

@test "${BATS_TEST_NUMBER}: \"${CMD} stringToBeReplaced replacementString nonexistent.docx\" prints an error message" {
  run ${DOCXBOX_BINARY} rpt nonexistent.docx
  [ "$status" -ne 0 ]

  source ./test/functional/_check_for_valgrind_errors.sh

  ${DOCXBOX_BINARY} rpt nonexistent.docx Lorem Dorem 2>&1 | tee "${ERR_LOG}"

  source ./test/functional/_check_for_valgrind_errors.sh

  cat "${ERR_LOG}" | grep --count "docxBox Error - File not found:"
}

@test "${BATS_TEST_NUMBER}: \"${CMD} stringToBeReplaced replacementString wrong_file_type\" prints an error message" {
  local pattern="docxBox Error - File is no ZIP archive:"
  local wrong_file_types=(
  "test/tmp/cp_lorem_ipsum.pdf"
  "test/tmp/cp_mock_csv.csv"
  "test/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    ${DOCXBOX_BINARY} rpt "${i}" Lorem Dorem 2>&1 | tee "${ERR_LOG}"
    source ./test/functional/_check_for_valgrind_errors.sh
    cat "${ERR_LOG}" | grep --count "${pattern}"
  done
}
