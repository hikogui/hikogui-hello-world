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
    --output=data/locale/hello_world.pot \
    -language=C++ \
    --keyword=l10n:1g \
    --keyword=l10p:1g,1g

msginit --no-translator --input=data/locale/hello_world.pot --output-file=data/locale/en.po
msgmerge --update data/locale/nl.po data/locale/hello_world.pot

rm /tmp/source_files_$$.txt

