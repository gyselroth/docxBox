#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

VALGRIND_LOG="test/tmp/mem-leak.log"
VALGRIND="valgrind -v --leak-check=full\
 --log-file=${VALGRIND_LOG}"

VALGRIND_ERR_PATTERN="ERROR SUMMARY: [0-9] errors from [0-9] contexts"

if $IS_VALGRIND_TEST;
then
  DOCXBOX_BINARY="${VALGRIND} $BATS_TEST_DIRNAME/../tmp/docxbox"
else
  DOCXBOX_BINARY="$BATS_TEST_DIRNAME/../tmp/docxbox"
fi

PATH_DOCX="test/tmp/cp_table_unordered_list_images.docx"
PATH_DOCX_NEW="test/tmp/cp_plain_text.docx"
ERR_LOG="test/tmp/err.log"

ERROR="is an error message"
DISPLAY_FILE="word/document.xml"

BASE_COMMAND="docxbox rpt filename.docx"

@test "Output of \"docxbox rpt {missing filename}\" ${ERROR}" {
  run ${DOCXBOX_BINARY} rpt
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: DOCX filename" = "${lines[0]}" ]

  check_for_valgrind_error
}

@test "Output of \"${BASE_COMMAND} {missing arguments}\" ${ERROR}" {
  pattern="docxBox Error - Missing argument: String to be found (and replaced)"

  run ${DOCXBOX_BINARY} rpt "${PATH_DOCX}"
  [ "$status" -ne 0 ]
  [ "${pattern}" = "${lines[0]}" ]

  check_for_valgrind_error
}

missing_argument="stringToBeReplaced {missing argument}"
@test "Output of \"${BASE_COMMAND} ${missing_argument}\" ${ERROR}" {
  run ${DOCXBOX_BINARY} rpt "${PATH_DOCX}" toBeReplaced
  [ "$status" -ne 0 ]
  [ "docxBox Error - Missing argument: Replacement" = "${lines[0]}" ]

  check_for_valgrind_error
}

ARGUMENTS="stringToBeReplaced replacementString"
appendix="the stringToBeReplaced gets replaced"
@test "With \"${BASE_COMMAND} ${ARGUMENTS}\" ${appendix}" {
  run ${DOCXBOX_BINARY} rpt "${PATH_DOCX}" Lorem Dorem
  [ "$status" -eq 0 ]

  check_for_valgrind_error

  ${DOCXBOX_BINARY} txt "${PATH_DOCX}" | grep --count Dorem
}

arguments_new_docx="stringToBeReplaced replacementString newFile.docx"
appendix_new_docx="the stringToBeReplaced gets replaced and is saved to new file"
@test "With \"${BASE_COMMAND} ${arguments_new_docx}\" ${appendix_new_docx}" {
  path_docx_out="test/tmp/replacedString.docx"

  run ${DOCXBOX_BINARY} rpt "${PATH_DOCX}" Lorem Dorem "${path_docx_out}"
  [ "$status" -eq 0 ]

  check_for_valgrind_error

  ${DOCXBOX_BINARY} txt "${path_docx_out}" | grep --count Dorem
}

title_h1="With \"${BASE_COMMAND} heading_as_JSON\" the given string is \
replaced by a h1"
@test "${title_h1}" {
  heading="{\"h1\":{\"text\":\"Foo\"}}"

  ${DOCXBOX_BINARY} cat "${PATH_DOCX}" "${DISPLAY_FILE}" | grep --invert-match "<w:pStyle w:val=\"para1\"/>"

  run ${DOCXBOX_BINARY} rpt "${PATH_DOCX}" Officia ${heading}
  [ "$status" -eq 0 ]

  check_for_valgrind_error

  ${DOCXBOX_BINARY} cat "${PATH_DOCX}" "${DISPLAY_FILE}" | grep --count "<w:pStyle w:val=\"para1\"/>"
  ${DOCXBOX_BINARY} lsl "${PATH_DOCX}" "para1" | grep --count "${DISPLAY_FILE}"
}

title_h2="With \"${BASE_COMMAND} heading_as_JSON\" the given string is \
replaced by a h2"
@test "${title_h2}" {
  heading="{\"h2\":{\"text\":\"Foo\"}}"

  ${DOCXBOX_BINARY} cat "${PATH_DOCX}" "${DISPLAY_FILE}" | grep --invert-match "<w:pStyle w:val=\"para2\"/>"

  run ${DOCXBOX_BINARY} rpt "${PATH_DOCX}" Officia ${heading}
  [ "$status" -eq 0 ]

  check_for_valgrind_error

  ${DOCXBOX_BINARY} cat "${PATH_DOCX}" "${DISPLAY_FILE}" | grep --count "<w:pStyle w:val=\"para2\"/>"
  ${DOCXBOX_BINARY} lsl "${PATH_DOCX}" "para2" | grep --count "${DISPLAY_FILE}"
}

title_h3="With \"${BASE_COMMAND} heading_as_JSON\" the given string is \
replaced by a h3"
@test "${title_h3}" {
  heading="{\"h3\":{\"text\":\"Foo\"}}"

  ${DOCXBOX_BINARY} cat "${PATH_DOCX}" "${DISPLAY_FILE}" | grep --invert-match "<w:pStyle w:val=\"para3\"/>"

  run ${DOCXBOX_BINARY} rpt "${PATH_DOCX}" Officia ${heading}
  [ "$status" -eq 0 ]

  check_for_valgrind_error

  ${DOCXBOX_BINARY} cat "${PATH_DOCX}" "${DISPLAY_FILE}" | grep --count "<w:pStyle w:val=\"para3\"/>"
  ${DOCXBOX_BINARY} lsl "${PATH_DOCX}" "para3" | grep --count "${DISPLAY_FILE}"
}

title_ul="With \"${BASE_COMMAND} ul_as_JSON\" the given string is replaced by \
a unordered list"
@test "${title_ul}" {
  list="{\"ul\":{\"items\":[\"item-1\",\"item-2\",\"item-3\"]}}"

  ${DOCXBOX_BINARY} cat "${PATH_DOCX_NEW}" "${DISPLAY_FILE}" | grep --invert-match "<w:numId w:val=\"1\"/>"

  run ${DOCXBOX_BINARY} rpt "${PATH_DOCX_NEW}" TITLE ${list}
  [ "$status" -eq 0 ]

  check_for_valgrind_error

  ${DOCXBOX_BINARY} cat "${PATH_DOCX_NEW}" "${DISPLAY_FILE}" | grep --count "<w:numId w:val=\"1\"/>"
  ${DOCXBOX_BINARY} lsl "${PATH_DOCX_NEW}" "w:numPr" | grep --count "${DISPLAY_FILE}"
}

title_hyperlink="With \"${BASE_COMMAND} hyperlink_as_JSON\" the given string \
is replaced by a hyperlink"
@test "${title_hyperlink}" {
  url="\"url\":\"https://github.com/gyselroth/docxbox\""
  link="{\"link\":{\"text\":\"docxBox\",${url}}}"

  ${DOCXBOX_BINARY} cat "${PATH_DOCX}" "${DISPLAY_FILE}" | grep --invert-match "</w:hyperlink>"

  run ${DOCXBOX_BINARY} rpt "${PATH_DOCX}" Officia ${link}
  [ "$status" -eq 0 ]

  check_for_valgrind_error

  ${DOCXBOX_BINARY} cat "${PATH_DOCX}" "${DISPLAY_FILE}" | grep --count "</w:hyperlink>"
  ${DOCXBOX_BINARY} lsl "${PATH_DOCX}" "</w:hyperlink>" | grep --count "${DISPLAY_FILE}"
}

image_replacement="the given string is replaced by an image"
@test "With \"${BASE_COMMAND} image_as_JSON\" ${image_replacement} using EMU" {
  image="{\"image\":{\"size\":[2438400,1828800]}}"
  image_path="test/assets/images/2100x400.jpeg"

  ${DOCXBOX_BINARY} cat "${PATH_DOCX}" "${DISPLAY_FILE}" | grep --invert-match "<w:drawing>"
  ${DOCXBOX_BINARY} cat "${PATH_DOCX}" "${DISPLAY_FILE}" | grep --invert-match "</w:drawing>"

  run ${DOCXBOX_BINARY} rpt "${PATH_DOCX}" Officia ${image} ${image_path}
  [ "$status" -eq 0 ]

  check_for_valgrind_error

  ${DOCXBOX_BINARY} cat "${PATH_DOCX}" "${DISPLAY_FILE}" | grep --count "<w:drawing>"
  ${DOCXBOX_BINARY} cat "${PATH_DOCX}" "${DISPLAY_FILE}" | grep --count "</w:drawing>"
  ${DOCXBOX_BINARY} lsl "${PATH_DOCX}" "<w:drawing>" | grep --count "${DISPLAY_FILE}"
  ${DOCXBOX_BINARY} lsl "${PATH_DOCX}" "</w:drawing>" | grep --count "${DISPLAY_FILE}"
}

@test "With \"${BASE_COMMAND} image_as_JSON\" ${image_replacement} using pixels" {
  image="{\"image\":{\"size\":[\"2100px\",\"400px\"]}}"
  image_path="test/assets/images/2100x400.jpeg"

  ${DOCXBOX_BINARY} cat "${PATH_DOCX}" "${DISPLAY_FILE}" | grep --invert-match "<w:drawing>"
  ${DOCXBOX_BINARY} cat "${PATH_DOCX}" "${DISPLAY_FILE}" | grep --invert-match "</w:drawing>"

  run ${DOCXBOX_BINARY} rpt "${PATH_DOCX}" Officia ${image} ${image_path}
  [ "$status" -eq 0 ]

  check_for_valgrind_error

  ${DOCXBOX_BINARY} cat "${PATH_DOCX}" "${DISPLAY_FILE}" | grep --count "<w:drawing>"
  ${DOCXBOX_BINARY} cat "${PATH_DOCX}" "${DISPLAY_FILE}" | grep --count "</w:drawing>"
  ${DOCXBOX_BINARY} lsl "${PATH_DOCX}" "<w:drawing>" | grep --count "${DISPLAY_FILE}"
  ${DOCXBOX_BINARY} lsl "${PATH_DOCX}" "</w:drawing>" | grep --count "${DISPLAY_FILE}"
}

title_table="With \"${BASE_COMMAND} table_as_JSON\" the given string is \
replaced by a table"
@test "${title_table}" {
  header="\"header\":[\"header_one\",\"B\",\"C\"]"
  first_column="[\"a1\",\"a2\",\"a3\"]"
  second_column="[\"b1\",\"b2\",\"b3\"]"
  third_column="[\"c1\",\"c2\",\"c3\"]"
  content="\"content\":[${first_column},${second_column},${third_column}]"
  table="{\"table\":{${header},${content}}}"

  pattern="<w:tblStyle w:val=\"TableGrid\"/>"

  ${DOCXBOX_BINARY} cat "${PATH_DOCX}" "${DISPLAY_FILE}" | grep --invert-match "${pattern}"

  run ${DOCXBOX_BINARY} rpt "${PATH_DOCX}" Officia ${table}
  [ "$status" -eq 0 ]

  check_for_valgrind_error

  ${DOCXBOX_BINARY} cat "${PATH_DOCX}" "${DISPLAY_FILE}" | grep --count "${pattern}"
  ${DOCXBOX_BINARY} lsl "${PATH_DOCX}" "<w:tbl>" | grep --count "${DISPLAY_FILE}"
}

title_table_as_JSON="With \"${BASE_COMMAND} table_as_JSON\" the given string \
is replaced by a table with header"
@test "${title_table_as_JSON}" {
  header="\"header\":[\"header_one\",\"B\",\"C\"]"
  first_column="[\"a1\",\"a2\",\"a3\"]"
  second_column="[\"b1\",\"b2\",\"b3\"]"
  third_column="[\"c1\",\"c2\",\"c3\"]"
  content="\"content\":[${first_column},${second_column},${third_column}]"
  table="{\"table\":{${header},${content}}}"

  pattern="<w:tblStyle w:val=\"TableGrid\"/>"

  ${DOCXBOX_BINARY} cat "${PATH_DOCX}" "${DISPLAY_FILE}" | grep --invert-match "${pattern}"

  run ${DOCXBOX_BINARY} rpt "${PATH_DOCX}" Officia ${table}
  [ "$status" -eq 0 ]

  check_for_valgrind_error

  ${DOCXBOX_BINARY} lsl "${PATH_DOCX}" "header_one" | grep --count -q "${DISPLAY_FILE}"
  ${DOCXBOX_BINARY} cat "${PATH_DOCX}" "${DISPLAY_FILE}" | grep --count -q "header_one"
}

title_case_sensitive="With \"${BASE_COMMAND} lower_case_string\" the given \
string gets replaced case sensitive"
@test "${title_case_sensitive}" {
  run ${DOCXBOX_BINARY} rpt "${PATH_DOCX_NEW}" "text" "FooBar"
  [ "$status" -eq 0 ]

  check_for_valgrind_error

  ${DOCXBOX_BINARY} txt "${PATH_DOCX_NEW}" | grep --count "Text"
  ${DOCXBOX_BINARY} txt "${PATH_DOCX_NEW}" | grep --count "FooBar"
}

@test "Output of \"docxbox rpt ${ARGUMENTS} nonexistent.docx\" ${ERROR}" {
  run ${DOCXBOX_BINARY} rpt nonexistent.docx
  [ "$status" -ne 0 ]

  check_for_valgrind_error

  ${DOCXBOX_BINARY} rpt nonexistent.docx Lorem Dorem 2>&1 | tee "${ERR_LOG}"

  check_for_valgrind_error

  cat "${ERR_LOG}" | grep --count "docxBox Error - File not found:"
}

@test "Output of \"docxbox rpt ${ARGUMENTS} wrong_file_type\" ${ERROR}" {
  pattern="docxBox Error - File is no ZIP archive:"
  wrong_file_types=(
  "test/tmp/cp_lorem_ipsum.pdf"
  "test/tmp/cp_mock_csv.csv"
  "test/tmp/cp_mock_excel.xls")

  for i in "${wrong_file_types[@]}"
  do
    ${DOCXBOX_BINARY} rpt "${i}" Lorem Dorem 2>&1 | tee "${ERR_LOG}"
    check_for_valgrind_error
    cat "${ERR_LOG}" | grep --count "${pattern}"
  done
}

check_for_valgrind_error() {
  if $IS_VALGRIND_TEST; then
    cat "${VALGRIND_LOG}" | grep --count --invert-match "${VALGRIND_ERR_PATTERN}"
  fi
}
