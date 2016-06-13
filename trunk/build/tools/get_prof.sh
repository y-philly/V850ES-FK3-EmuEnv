#!/bin/sh


cp profTemplate.xlsx prof.xlsx

SRC_FILE="../prof.csv"
SRC_SHEET="prof"
SRC_RANGE="a2:d500"

DST_FILE="prof.xlsx"
DST_SHEET="prof"
DST_RANGE="a2:d500"

ruby ../gui_debugger/excel_range_copy.rb  ${SRC_FILE} ${SRC_SHEET} ${SRC_RANGE} ${DST_FILE} ${DST_SHEET} ${DST_RANGE}

