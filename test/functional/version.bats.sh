#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper
source ./test/functional/_set_docxbox_binary.sh

@test "${BATS_TEST_NUMBER}: \"docxbox v\" displays version number" {
  pattern="(^|\s)+(docxBox version )\K([0-9]|\.)*(?=\s|$)"

  ${DOCXBOX_BINARY} v | grep -Po "${pattern}"

  source ./test/functional/_check_for_valgrind_errors.sh
}
