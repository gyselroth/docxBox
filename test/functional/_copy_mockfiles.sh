#!/usr/bin/env bash

if [ ! -d test/functional/tmp ]; then
    mkdir test/functional/tmp;
fi

for f in test/files/docx/*.docx
  do
    cp $f test/functional/tmp/cp_${f##*/};
  done
