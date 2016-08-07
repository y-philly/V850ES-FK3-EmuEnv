TARGET	:= cpu_emulator
ROOTDIR	:= ..

ifdef USE_CANLIB
KVASERDIR := /c/Program\ Files\ \(x86\)/Kvaser/Canlib
endif

ifdef USE_CANLIB_X86
USE_CANLIB := TRUE
KVASERDIR := /c/Program\ Files/Kvaser/Canlib
endif

IFLAGS	:= -I$(ROOTDIR)/main
IFLAGS	+= -I$(ROOTDIR)/cpu
IFLAGS	+= -I$(ROOTDIR)/elf_loader
IFLAGS	+= -I$(ROOTDIR)/cpu/exec
IFLAGS	+= -I$(ROOTDIR)/debugger/device/can
IFLAGS	+= -I$(ROOTDIR)/inc
IFLAGS	+= -I$(ROOTDIR)/lib
IFLAGS	+= -I$(KVASERDIR)/INC
IFLAGS	+= -I$(ROOTDIR)

WFLAGS	:= -g -Wall

GCC		:= gcc

VPATH	:= $(ROOTDIR)/main
VPATH	+= $(ROOTDIR)/elf_loader
VPATH	+= $(ROOTDIR)/cpu
VPATH	+= $(ROOTDIR)/cpu/exec
VPATH	+= $(ROOTDIR)/intc
VPATH	+= $(ROOTDIR)/debugger
VPATH	+= $(ROOTDIR)/device
VPATH	+= $(ROOTDIR)/device/timer
VPATH	+= $(ROOTDIR)/device/timer_m
VPATH	+= $(ROOTDIR)/device/serial
VPATH	+= $(ROOTDIR)/device/can
VPATH	+= $(ROOTDIR)/device/adc
VPATH	+= $(ROOTDIR)/device/wdg
VPATH	+= $(ROOTDIR)/lib

ifdef USE_CANLIB
VPATH	+= $(ROOTDIR)/debugger/device/can/kvaser
else
VPATH	+= $(ROOTDIR)/debugger/device/can/stdio
endif

CFLAGS	:= $(WFLAGS)
CFLAGS	+= $(IFLAGS)

#LFLAGS	:= -mwindows -Wl,--allow-multiple-definition 
LFLAGS	:= -Wl,--allow-multiple-definition 
LFLAGS	+= -Wl,--enable-stdcall-fixup 

LIBS	:= -lpthread
ifdef USE_CANLIB
LIBS	+= $(KVASERDIR)/Bin/canlib32.dll
endif

OBJS	:= main.o
OBJS	+= cpu.o
OBJS	+= op_format.o
OBJS	+= op_dec.o
OBJS	+= op_exec.o
OBJS	+= op_exec_arithm.o
OBJS	+= op_exec_bit.o
OBJS	+= op_exec_branch.o
OBJS	+= op_exec_dbg.o
OBJS	+= op_exec_div.o
OBJS	+= op_exec_load.o
OBJS	+= op_exec_logic.o
OBJS	+= op_exec_sat.o
OBJS	+= op_exec_spec.o
OBJS	+= op_exec_store.o
OBJS	+= elf_loader.o
OBJS	+= intc.o
OBJS	+= device.o
OBJS	+= timer.o
OBJS	+= timer_m.o
OBJS	+= dbg_can.o
OBJS	+= can.o
OBJS	+= adc.o
OBJS	+= wdg.o
OBJS	+= serial.o
OBJS	+= debugger.o
OBJS	+= symbol.o
OBJS	+= symbol_ops.o
OBJS	+= hash.o


.SUFFIXES:	.c .o
.SUFFIXES:	.s .o

all:	$(TARGET)

$(TARGET):	$(OBJS)
	$(GCC) -O3 $(LFLAGS) $(OBJS) -o $(TARGET)  $(LIBS)

.c.o:	$<
	$(GCC) -O3 -c $(CFLAGS) $<

.s.o:	$<
	$(GCC) -O3 -c $(CFLAGS) $<

clean:
	rm -f $(OBJS) $(TARGET).exe 
