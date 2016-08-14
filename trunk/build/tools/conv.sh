#!/bin/sh

if [ $# -ne 1 ]
then
    echo "Usage: $0 elf"
    exit 1
fi

ELF=${1}

v850-elf-objcopy.exe -O binary ${ELF} ${ELF}.bin

v850-elf-objdump.exe -S -d ${ELF} > ${ELF}.dump

#nkf32 -s -f ${ELF}.dump > ../binary.dump
