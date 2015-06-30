#!/bin/sh
set -e

STDJ="-j4"
MAKE=${MAKE-make} # gmake
RUMPSRC=${PWD}/../src-HEAD

RUMPOBJ=${PWD}/rumpobj
RUMP=${RUMPOBJ}/rump
RUMPTOOLS=${RUMPOBJ}/tooldir
RUMPMAKE=${RUMPTOOLS}/rumpmake

BUILDRUMP=${PWD}/buildrump.sh

die ()
{
	echo '>>' $*
	exit 1
}

buildrump()
{
	${BUILDRUMP}/buildrump.sh \
		-V MKPIC=no -V RUMP_CURLWP=__thread			\
		-V RUMP_KERNEL_IS_LIBC=1				\
		${STDJ} -k -s ${RUMPSRC} -o ${RUMPOBJ} -d ${RUMP}	\
		tools build kernelheaders install
}

buildrumpuser()
{
	for lib in libbmk_core libbmk_rumpuser; do
		MACHINE=${MACHINE} ${RUMPMAKE} -C librumpuser/lib/$lib ${1} 
	done
}

buildhw()
{
	( cd hw && ${MAKE} MACHINE=${MACHINE} RUMP=$RUMP ${1} || exit 1)
}

clean()
{
	buildhw clean
	buildrumpuser clean
	rm -rf "${RUMPOBJ}"
}

# TODO clean this mess up, use autodetection
export MACHINE=riscv
export CC=riscv64-unknown-linux-gnu-gcc
export NM=riscv64-unknown-linux-gnu-nm
export AR=riscv64-unknown-linux-gnu-ar
export LD=riscv64-unknown-linux-gnu-gcc
export OBJCOPY=riscv64-unknown-linux-gnu-objcopy

buildrump
buildrumpuser
buildhw
