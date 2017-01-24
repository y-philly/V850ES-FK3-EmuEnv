#!/bin/sh

if [ $# -ne 2 ]
then
	echo "Usage: $0 test_item template.csv"
	exit 1
fi

IFLAGS="-I ./TestObject -I ./TestObject/target -I ./TestObject/item -I ./TestObjectFactory -I ./TestCodeGenerator -I ./util"


ruby ${IFLAGS} TestCodeGenerator/TestCodeGenerator.rb ${1} ${2}

