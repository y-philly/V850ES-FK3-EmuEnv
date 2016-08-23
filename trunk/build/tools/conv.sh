#!/bin/sh

if [ $# -ne 1 ]
then
    echo "Usage: $0 elf"
    exit 1
fi

ELF=${1}

v850-elf-objcopy.exe -O binary ${ELF} ${ELF}.bin
#v850-elf-objdump.exe -S -d ${ELF} > ${ELF}.dump
v850-elf-objdump.exe -l -D ${ELF} > ${ELF}.dump

sh elf2symbol.sh ${ELF}

sh create_debuginfo.sh ${ELF}.dump

cd ..
#make
make USE_CANLIB=TRUE

