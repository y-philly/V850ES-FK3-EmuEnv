#!/bin/sh

if [ $# -ne 1 ]
then
    echo "Usage: $0 elf_file"
    exit 1
fi

ELF_FILE=${1}

readelf -l ${ELF_FILE} | grep LOAD | grep "RW"  |  awk '{print $6}' | awk -Fx '{ print $2 ",RW," }' > mem.csv
readelf -l ${ELF_FILE} | grep LOAD | grep "R E" |  awk '{print $6}' | awk -Fx '{ print $2 ",RE,"}'  >> mem.csv
readelf -l ${ELF_FILE} | grep LOAD | grep "R  " |  awk '{print $6}' | awk -Fx '{ print $2 ",R,"}'  >> mem.csv


cp memTemplate.xlsx mem.xlsx

SRC_FILE="./mem.csv"
SRC_SHEET="mem"
SRC_RANGE="a1:b50"

DST_FILE="./mem.xlsx"
DST_SHEET="mem"
DST_RANGE="b6:c55"

ruby ../gui_debugger/excel_range_copy.rb  ${SRC_FILE} ${SRC_SHEET} ${SRC_RANGE} ${DST_FILE} ${DST_SHEET} ${DST_RANGE}
