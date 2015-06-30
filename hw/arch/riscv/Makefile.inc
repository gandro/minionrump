OBJS_BMK+=	arch/riscv/cpu.o arch/riscv/htifcons.o arch/riscv/locore.o
LDSCRIPT=	arch/riscv/kern.ldscript

arch/riscv/locore.o: arch/riscv/locore.S
	${CC} ${CFLAGS} ${CPPFLAGS} -Iinclude -D_LOCORE -c -o $@ $<
