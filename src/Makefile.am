SUBDIRS = kaleido		\
	      core			\
	      export		\
	      widgets		\
	      share/images
	      
INCLUDES = -I$(top_srcdir)				\
		   -I$(top_srcdir)/src/kaleido	\
		   -I$(top_srcdir)/src/widgets	\
		   -I$(top_srcdir)/src/export	\
		   -I$(top_srcdir)/src/core

if HAVE_WINDRES
SAAICONRC = saa-rc.o
endif


bin_PROGRAMS = gsolaar solaar

solaar_SOURCES = solaar.c
solaar_LDADD = export/libsaaexport.a core/libsaacore.a kaleido/libkaleido.a

gsolaar_SOURCES = gsolaar.c #resources.c gsolaar-logo.ico
gsolaar_LDADD = widgets/libsaawidgets.a export/libsaaexport.a core/libsaacore.a kaleido/libkaleido.a $(SAAICONRC)

saa-rc.o : saa-rc.c gsolaar-logo.ico
	windres -i $< -o $@

