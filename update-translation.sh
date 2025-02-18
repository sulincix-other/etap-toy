#!/bin/bash

if ! command -v xgettext &> /dev/null
then
	echo "xgettext could not be found."
	echo "you can install the package with 'apt install gettext' command on debian."
	exit
fi


echo "updating pot file"
xgettext -o po/toy.pot --from-code="utf-8" \
    `find toy -type f -iname "*.py"` \
    `find toy -type f -iname "*.ui"`

for lang in $(cat po/LINGUAS); do
	if [[ -f po/$lang.po ]]; then
		echo "updating $lang.po"
		msgmerge -o po/$lang.po po/$lang.po po/toy.pot
	else
		echo "creating $lang.po"
		cp po/toy.pot po/$lang.po
	fi
done
