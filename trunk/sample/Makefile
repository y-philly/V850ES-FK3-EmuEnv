TARGET	:=	sample.elf

CC		:= v850-elf-gcc
LINK	:= v850-elf-gcc

#
#   �I�u�W�F�N�g�t�@�C����`
#
OBJS	:= start.o
OBJS	+= main.o
OBJS	+= intc.o
OBJS	+= int_handler.o
OBJS	+= timer.o
OBJS	+= serial.o
OBJS	+= api.o
OBJS	+= vector.o


#
#   �R���p�C���I�v�V����
#
CFLAGS	:= -mdisable-callt -mno-app-regs -mtda=0
CFLAGS	+= -gdwarf-2 -Wall -Wno-unused-label -Wpointer-arith  -mv850es -Wa,-mno-bcond17 -msoft-float -Wa,-mwarn-signed-overflow -Wa,-mwarn-unsigned-overflow
#CGLAGS	+= -mcmodel=medium -Wall -O2


#
#  �����J�X�N���v�g
#
LDSCRIPT = ./v850esfk3.ld
LDFLAGS	:= -nostdlib -T $(LDSCRIPT)

IFLAGS	:= -I.

#
#  ���C�u����
#
LIBS := -lm -lgcc -lc
#LIBS := -lgcc -lc

all: $(OBJS)
	$(LINK) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJS) -Wl,-Map,$(TARGET).map

vector.o: vector.S
	$(CC) -c $(IFLAGS) $(CFLAGS) $<

start.o: start.S
	$(CC) -c $(IFLAGS) $(CFLAGS) $<

main.o: main.c
	$(CC) -c $(IFLAGS) $(CFLAGS) $<

timer.o: timer.c
	$(CC) -c $(IFLAGS) $(CFLAGS) $<


clean:
	rm -f $(OBJS) $(TARGET) *.map
