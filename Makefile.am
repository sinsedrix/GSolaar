SUBDIRS = po src
#SUBDIRS = po src tu/core tu/widgets

man_MANS = man/solaar.1

EXTRA_DIST = config.rpath m4/ChangeLog  $(man_MANS)	\
			 AUTHORS-en.txt	\
			 COPYING-en.txt \
			 INSTALL-en.txt \
			 AUTHORS-fr.txt \
             COPYING-fr.txt \
             INSTALL-fr.txt \
			 TODO.txt

ACLOCAL_AMFLAGS = -I m4

deliver: dist
	tar xzf $(PACKAGE)-$(VERSION).tar.gz
	tar cjf $(PACKAGE)-$(VERSION).tar.bz2 $(PACKAGE)-$(VERSION)
	rm -rf $(PACKAGE)-$(VERSION)
	mv $(PACKAGE)-$(VERSION).tar.gz delivery
	cd packaging/win32 && $(MAKE) deliver
	cp $(PACKAGE)-$(VERSION).tar.bz2 ~/rpm/SOURCES/
	cd packaging/rpm && $(MAKE) deliver
	mv $(PACKAGE)-$(VERSION).tar.bz2 delivery

