#!/bin/sh

if [ $# -ne 2 ]
then
    echo "Usage: $0 elf proc"
    exit 1
fi

CPU_EMU=cpu_emulator.exe

ELF=${1}
PROC=${2}

v850-elf-objcopy.exe -O binary ${ELF} ${ELF}.bin
#v850-elf-objdump.exe -S -d ${ELF} > ${ELF}.dump
v850-elf-objdump.exe -l -D ${ELF} > ${ELF}.dump

sh elf2symbol.sh ${ELF}

sh create_debuginfo.sh ${ELF}.dump

cd ..
#make
make USE_CANLIB=FALSE

cd tools

rm -rf ../../bin/${PROC}/*
cp ../asm.dump ../../bin/${PROC}/
cp ../asm_line.txt ../../bin/${PROC}/
touch ../../bin/${PROC}/fifo
cp ../${CPU_EMU} ../../bin/${PROC}/${CPU_EMU}
cp ${ELF}* ../../bin/${PROC}/
cp -rp ../tools ../../bin/${PROC}/tools
cp -rp ../gui_debugger ../../bin/${PROC}/gui_debugger
cp -rp ../can_data_creator ../../bin/${PROC}/can_data_creator


