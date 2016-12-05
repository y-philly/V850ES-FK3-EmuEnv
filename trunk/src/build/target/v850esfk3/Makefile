ROOTDIR	:= ../../..
ARCH	:=v850esfk3

IFLAGS	:=	-I$(ROOTDIR)/device/intc/target/$(ARCH)
IFLAGS	+=	-I$(ROOTDIR)/device/peripheral/target/$(ARCH)

VPATH	:=	$(ROOTDIR)/cpu/target/$(ARCH)/cpu_common
VPATH	+=	$(ROOTDIR)/cpu/target/$(ARCH)/cpu_dec
VPATH	+=	$(ROOTDIR)/cpu/target/$(ARCH)/cpu_exec
VPATH	+=	$(ROOTDIR)/device/intc/target/$(ARCH)
VPATH	+=	$(ROOTDIR)/device/peripheral/target/$(ARCH)
VPATH	+=	$(ROOTDIR)/device/peripheral/target/$(ARCH)/adc
VPATH	+=	$(ROOTDIR)/device/peripheral/target/$(ARCH)/timer
VPATH	+=	$(ROOTDIR)/device/peripheral/target/$(ARCH)/timer_m
VPATH	+=	$(ROOTDIR)/device/peripheral/target/$(ARCH)/serial
VPATH	+=	$(ROOTDIR)/device/peripheral/target/$(ARCH)/comm
VPATH	+=	$(ROOTDIR)/device/peripheral/target/$(ARCH)/can
VPATH	+=	$(ROOTDIR)/device/target/$(ARCH)

CFLAGS	:=

LFLAGS	:=

LIBS	:=

OBJS	:=	op_exec.o
OBJS	+=	op_dec.o
OBJS	+=	op_format.o
OBJS	+=	op_exec_bit.o
OBJS	+=	op_exec_branch.o
OBJS	+=	op_exec_dbg.o
OBJS	+=	op_exec_div.o
OBJS	+=	op_exec_logic.o
OBJS	+=	op_exec_sat.o
OBJS	+=	op_exec_arithm.o
OBJS	+=	op_exec_store.o
OBJS	+=	op_exec_load.o
OBJS	+=	op_exec_spec.o
OBJS	+=	device.o
OBJS	+=	adc.o
OBJS	+=	timer.o
OBJS	+=	timer_m.o
OBJS	+=	serial.o
OBJS	+=	comm.o
OBJS	+=	can.o

include ../../Makefile.common

