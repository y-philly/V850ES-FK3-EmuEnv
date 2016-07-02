#!/bin/sh

if [ $# -ne 1 ]
then
    echo "Usage: $0 dump_file"
    exit 1
fi

DUMP_FILE=${1}

cp ${DUMP_FILE} ../asm.dump
#egrep -e ' [0-9a-f]+:|<[a-zA-Z_0-9]+>:' ${DUMP_FILE} | grep -v \* > ../asm_line.txt
egrep -n -e ' [0-9a-f]+:' ${DUMP_FILE} | grep -v \* > ../asm_line.txt

cd ../gui_debugger

cp DebuggerTemplate.xlsx Debugger.xlsx
ruby debugger_v850.rb init
cp symbol.c ../


SRC_FILE="asm.csv"
SRC_SHEET="asm"
SRC_RANGE="a1:a46000"

DST_FILE="Debugger.xlsx"
DST_SHEET="asm"
DST_RANGE="b1:b46000"

ruby excel_range_copy.rb  ${SRC_FILE} ${SRC_SHEET} ${SRC_RANGE} ${DST_FILE} ${DST_SHEET} ${DST_RANGE}

cd ../tools

