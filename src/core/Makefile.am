INCLUDES = -I$(top_srcdir)/src/kaleido

noinst_LIBRARIES = libsaacore.a

libsaacore_a_SOURCES = saa-core.h       saa-i18n.h          \
					   saa-utils.c		saa-utils.h			\
					   saa-geometrie.c	saa-geometrie.h		\
					   saa-solide.c		saa-solide.h		\
					   saa-traitement.c	saa-traitement.h	\
					   saa-visuel.c		saa-visuel.h

datadir = @datadir@
localedir = $(datadir)/locale
DEFS =  -DLOCALEDIR=\"$(localedir)\"      \
        -DPREFIX=\"$(prefix)\"            \
        -DBINDIR=\"$(bindir)\"            \
        -D_INTL_REDIRECT_INLINE  @DEFS@

# DEFS = -DLOCALEDIR=\"$(localedir)\" @DEFS@
# AM_CFLAGS =
