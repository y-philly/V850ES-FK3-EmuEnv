ROOTDIR	:= ../../..
ARCH	:=v850esfk3

IFLAGS	:=
VPATH	:=	$(ROOTDIR)/cpu/target/$(ARCH)/cpu_common
VPATH	+=	$(ROOTDIR)/cpu/target/$(ARCH)/cpu_dec
VPATH	+=	$(ROOTDIR)/cpu/target/$(ARCH)/cpu_exec
CFLAGS	:=
LFLAGS	:=
LIBS	:=
OBJS	:=	op_exec.o
OBJS	+=	op_dec.o
OBJS	+=	op_format.o
OBJS	+= op_exec_bit.o
OBJS	+= op_exec_branch.o
OBJS	+= op_exec_dbg.o
OBJS	+= op_exec_div.o
OBJS	+= op_exec_logic.o
OBJS	+= op_exec_sat.o
OBJS	+= op_exec_arithm.o
OBJS	+= op_exec_store.o
#OBJS	+= op_exec_spec.o
#OBJS	+= op_exec_load.o

include ../../Makefile.common

