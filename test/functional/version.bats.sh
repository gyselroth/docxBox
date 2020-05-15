#!/usr/bin/env bats

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

load _helper

@test "\"docxbox v\" displays version number" {
  pattern="(^|\s)+(docxBox version )\K([0-9]|\.)*(?=\s|$)"

  "$BATS_TEST_DIRNAME"/docxbox v | grep -Po "${pattern}"
}
