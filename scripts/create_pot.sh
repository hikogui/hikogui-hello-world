#!/bin/sh

if [ -e scripts -a -e src -a -e data ]
then
    echo "Extracting translation strings from project"
else
    echo "Run this script in the project root."
    exit 2
fi

find src -name \*.hpp -or -name \*.cpp >/tmp/source_files_$$.txt

xgettext \
    --files-from=/tmp/source_files_$$.txt \
    --default-domain=hello \
    --force-po \
    --output=data/hello_world.pot \
    --language=C++ \
    --keyword=tr:1g \
    --keyword=tr:1g,1g

msginit --no-translator --input=data/hello_world.pot --output-file=resources/locale/en.po
msgmerge --update resources/locale/nl.po data/hello_world.pot

rm /tmp/source_files_$$.txt

