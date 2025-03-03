#!/bin/bash
set -e
xgettext -o po/etap-toy.pot --from-code="utf-8" \
    --language=C++ -v --keyword=_\
    `find src -type f -iname "*.cpp"`\
    `find src -type f -iname "*.c"`
for file in $(cat po/LINGUAS); do
    msgmerge po/$file.po po/etap-toy.pot -o po/$file.new
    echo POT: $file
    rm -f po/$file.po
    mv po/$file.new po/$file.po
done
