#!/usr/bin/env bats

load _helper

@test "\"docxbox v\" displays version number" {
  "$BATS_TEST_DIRNAME"/docxbox v | grep -Po "(^|\s)+(docxBox version )\K([0-9]|\.)*(?=\s|$)"
}
