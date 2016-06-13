#!/bin/sh

if [ $# -ne 1 ]
then
    echo "Usage: $0 dump_file"
    exit 1
fi

DUMP_FILE=${1}

cp ${DUMP_FILE} ../asm.dump

cd ../gui_debugger

cp DebuggerTemplate.xlsx Debugger.xlsx
ruby debugger_v850.rb init
cp symbol.c ../


SRC_FILE="asm.csv"
SRC_SHEET="asm"
SRC_RANGE="a3:d16000"

DST_FILE="Debugger.xlsx"
DST_SHEET="asm"
DST_RANGE="b5:e16000"

ruby excel_range_copy.rb  ${SRC_FILE} ${SRC_SHEET} ${SRC_RANGE} ${DST_FILE} ${DST_SHEET} ${DST_RANGE}

cd ../tools

