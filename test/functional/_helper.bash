#!/usr/bin/env bash

case "$OSTYPE" in
  bsd*)     OS="bsd" ;;
  darwin*)  OS="osx" ;;
  linux*)   OS="linux" ;;
  solaris*) OS="solaris" ;;
  *)        OS="unknown: $OSTYPE" ;;
esac

setup() {
  if [ "$OS" == "osx" ]; then
    cp $BATS_TEST_DIRNAME/../../bin/mac/docxbox $BATS_TEST_DIRNAME/docxbox
  elif [ $OS == "linux" ]; then
    cp $BATS_TEST_DIRNAME/../../bin/linux/docxbox $BATS_TEST_DIRNAME/docxbox
  fi
  if [ ! -f test/files/tbl.docx ]; then cp test/files/bio_assay.docx test/files/tbl.docx; fi
}

teardown() {
  if [ -f $BATS_TEST_DIRNAME/docxbox ] ; then rm $BATS_TEST_DIRNAME/docxbox; fi
  if [ -f test/files/tbl.docx ]; then rm test/files/tbl.docx; fi
}
