#!/usr/bin/env bash

# Copyright (c) 2020 gyselroth GmbH
# Licensed under the MIT License - https://opensource.org/licenses/MIT

#if [ ! -d test/tmp ]; then
#    mkdir test/tmp;
#fi

for f in test/assets/documents/docx/*.docx
  do
    cp "$f" test/tmp/cp_"${f##*/}";
  done

for f in test/assets/documents/other/*
  do
    cp "$f" test/tmp/cp_"${f##*/}";
  done
