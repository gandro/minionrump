# TODO cleanup. this makefile is rather hacky

all: include/bmk/machine minion.elf

CFLAGS=	-Iinclude -I../librumpuser/include -I${RUMP}/include -nostdinc
CFLAGS+= -std=gnu99 ${DBG}
CFLAGS+= -fno-stack-protector -ffreestanding
CFLAGS+= -Wall -Wimplicit -Wmissing-prototypes -Wstrict-prototypes
CFLAGS+= -D_KERNEL
DBG?=	 -O2 -g

OBJS_BMK+=		intr.o clock.o kernel.o multiboot.o undefs.o

LDFLAGS:= -L$(abspath ${RUMP}/lib)
LDFLAGS+= -L$(abspath ../librumpuser/lib/libbmk_core/)
LDFLAGS+= -L$(abspath ../librumpuser/lib/libbmk_rumpuser/)

include arch/${MACHINE}/Makefile.inc

OBJS_BMK+=	init.o

OBJS= ${OBJS_BMK}

.PHONY:	clean

include/bmk/machine:
	@ln -s ../arch/${MACHINE} include/bmk/machine

minion.elf: ${OBJS}
	${CC} ${LDFLAGS} -T${LDSCRIPT} -nostdlib 			\
	 ${OBJS} 							\
	-lrump -lbmk_core -lbmk_rumpuser				\
	-o $@

clean:
	rm -f ${OBJS} include/bmk/machine
