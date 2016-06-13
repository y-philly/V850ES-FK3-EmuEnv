#!/bin/sh


SRC_FILE="../gui_debugger/asm.csv"
SRC_SHEET="asm"
SRC_RANGE="a1:d16000"

DST_FILE="asm.xlsx"
DST_SHEET="asm"
DST_RANGE="a1:d16000"

cp ../gui_debugger/asm.xlsx .

ruby ../gui_debugger/excel_range_copy.rb  ${SRC_FILE} ${SRC_SHEET} ${SRC_RANGE} ${DST_FILE} ${DST_SHEET} ${DST_RANGE}
ruby cov.rb

