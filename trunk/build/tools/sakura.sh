#!/bin/sh

SAKURA_FILE="../gui_debugger/arg_sakura.txt"
LINE=`cat ${SAKURA_FILE} | awk -F: '{print $NF}' `
FILE_PATH=`cat ${SAKURA_FILE} | awk -F: '{print $1 ":" $2}' `

sakura  -Y=${LINE} ${FILE_PATH}
