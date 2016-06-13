#!/bin/sh

if [ $# -ne 1 ]
then
    echo "Usage: $0 dump_file"
    exit 1
fi

DUMP_FILE=${1}

cp ${DUMP_FILE} ../asm.dump

cd ../gui_debugger
ruby debugger_v850.rb init
cp symbol.c ../
cd ../tools

