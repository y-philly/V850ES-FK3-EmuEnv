#!/bin/sh

IFLAGS="-I ./TestObject -I ./TestObject/target -I ./TestObject/item -I ./TestObjectFactory -I ./TestCodeGenerator -I ./util"

ruby ${IFLAGS} TestCodeGenerator/TestCodeGenerator.rb ${1} ${2}

