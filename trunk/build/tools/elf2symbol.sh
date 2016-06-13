#!/bin/sh

if [ $# -ne 1 ]
then
    echo "Usage: $0 elf_file"
    exit 1
fi

ELF_FILE=${1}

readelf --debug-dump=decodedline ${ELF_FILE} > ../line.txt

readelf -s ${ELF_FILE} | grep OBJECT > ../symbol.txt
readelf -s ${ELF_FILE} | grep FUNC >> ../symbol.txt


