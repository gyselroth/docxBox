#!/usr/bin/env bash

case "$OSTYPE" in
  bsd*)     OS="bsd" ;;
  darwin*)  OS="osx" ;;
  linux*)   OS="linux" ;;
  solaris*) OS="solaris" ;;
  *)        OS="unknown: $OSTYPE" ;;
esac

# TODO(lucas): clean-up globally
setup() {
  if [ "$OS" == "osx" ]; then
    cp "$BATS_TEST_DIRNAME"/../../bin/mac/docxbox "$BATS_TEST_DIRNAME"/docxbox
  elif [ $OS == "linux" ]; then
    cp "$BATS_TEST_DIRNAME"/../../bin/linux/docxbox "$BATS_TEST_DIRNAME"/docxbox
  fi
  if [ ! -d test/functional/tmp ]; then
    mkdir test/functional/tmp;
  fi

# TODO(lucas): copy mock files using array of filenames via variable
#  or via wildcard (e.g. "*.docx") instead of hard-coding filenames
#  within build-up

  if [ ! -f test/functional/tmp/cp_bio_assay.docx ]; then
    cp test/files/docx/bio_assay.docx test/functional/tmp/cp_bio_assay.docx;
  fi

  if [ ! -f test/functional/tmp/cp_table_unordered_list_images.docx ]; then
    cp test/files/docx/table_unordered_list_images.docx test/functional/tmp/cp_table_unordered_list_images.docx;
  fi
}

teardown() {
  if [ -f "$BATS_TEST_DIRNAME"/docxbox ] ; then
    rm "$BATS_TEST_DIRNAME"/docxbox;
  fi
  if [ -d test/functional/tmp ]; then
    rm -r test/functional/tmp;
  fi
}
