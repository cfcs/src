#	$OpenBSD: Makefile.inc,v 1.11 2017/04/30 23:33:48 djm Exp $

REGRESS_FAIL_EARLY?=	yes

.include <bsd.own.mk>
.include <bsd.obj.mk>

MALLOC_OPTIONS?=	CFGJRSUX
TEST_ENV?=		MALLOC_OPTIONS=${MALLOC_OPTIONS}

# XXX detect from ssh binary?
OPENSSL?=	yes

.if (${OPENSSL:L} == "yes")
CFLAGS+=	-DWITH_OPENSSL
.endif

# enable warnings
WARNINGS=Yes

DEBUG=-g
CFLAGS+=	-fstack-protector-all
CDIAGFLAGS=	-Wall
CDIAGFLAGS+=	-Wextra
CDIAGFLAGS+=	-Werror
CDIAGFLAGS+=	-Wchar-subscripts
CDIAGFLAGS+=	-Wcomment
CDIAGFLAGS+=	-Wformat
CDIAGFLAGS+=	-Wformat-security
CDIAGFLAGS+=	-Wimplicit
CDIAGFLAGS+=	-Winline
CDIAGFLAGS+=	-Wmissing-declarations
CDIAGFLAGS+=	-Wmissing-prototypes
CDIAGFLAGS+=	-Wparentheses
CDIAGFLAGS+=	-Wpointer-arith
CDIAGFLAGS+=	-Wreturn-type
CDIAGFLAGS+=	-Wshadow
CDIAGFLAGS+=	-Wsign-compare
CDIAGFLAGS+=	-Wstrict-aliasing
CDIAGFLAGS+=	-Wstrict-prototypes
CDIAGFLAGS+=	-Wswitch
CDIAGFLAGS+=	-Wtrigraphs
CDIAGFLAGS+=	-Wuninitialized
CDIAGFLAGS+=	-Wunused
.if ${COMPILER_VERSION:L} != "gcc3"
CDIAGFLAGS+=	-Wpointer-sign
CDIAGFLAGS+=	-Wold-style-definition
.endif

SSHREL=../../../../../usr.bin/ssh

CFLAGS+=-I${.CURDIR}/../test_helper -I${.CURDIR}/${SSHREL}

.if exists(${.CURDIR}/../test_helper/${__objdir})
LDADD+=-L${.CURDIR}/../test_helper/${__objdir} -ltest_helper
DPADD+=${.CURDIR}/../test_helper/${__objdir}/libtest_helper.a
.else
LDADD+=-L${.CURDIR}/../test_helper -ltest_helper
DPADD+=${.CURDIR}/../test_helper/libtest_helper.a
.endif

.if exists(${.CURDIR}/${SSHREL}/lib/${__objdir})
LDADD+=-L${.CURDIR}/${SSHREL}/lib/${__objdir} -lssh
LIBSSH=${.CURDIR}/${SSHREL}/lib/${__objdir}/libssh.a
.else
LDADD+=-L${.CURDIR}/${SSHREL}/lib -lssh
LIBSSH=${.CURDIR}/${SSHREL}/lib/libssh.a
.endif
DPADD+=${LIBSSH}
${PROG}: ${LIBSSH}
${LIBSSH}:
	cd ${.CURDIR}/${SSHREL} && ${MAKE} lib

LDADD+= -lutil
DPADD+= ${LIBUTIL}

.if (${OPENSSL:L} == "yes")
LDADD+= -lcrypto
DPADD+= ${LIBCRYPTO}
.endif
