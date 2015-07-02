.include <bsd.own.mk>

LIB=	rumpdev_htif
IOCONF=	HTIF.ioconf

SRCS+=	htif.c htif_at_mainbus.c

RUMPTOP= ${TOPRUMP}

CPPFLAGS+=	-I${RUMPTOP}/librump/rumpkern
CPPFLAGS+=	-I${.CURDIR}


# XXX
.undef RUMPKERN_ONLY

.include "${RUMPTOP}/Makefile.rump"
.include <bsd.lib.mk>
.include <bsd.klinks.mk>
