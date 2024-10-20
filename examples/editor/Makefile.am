## Process this file with automake to produce Makefile.in

SUBDIRS = po src

DISTCLEANFILES = *~ autoscan.log build-aux/*~ m4/*~

install-data-local:
	@$(NORMAL_INSTALL)
	if test -d $(srcdir)/pixmaps; then \
	  $(mkinstalldirs) $(DESTDIR)$(pkgdatadir)/pixmaps; \
	  for pixmap in $(srcdir)/pixmaps/*; do \
	    if test -f $${pixmap}; then \
	      $(INSTALL_DATA) $${pixmap} $(DESTDIR)$(pkgdatadir)/pixmaps; \
	    fi \
	  done \
	fi

dist-hook:
	if test -d pixmaps; then \
	  mkdir $(distdir)/pixmaps; \
	  for pixmap in pixmaps/*; do \
	    if test -f $${pixmap}; then \
	      cp -p $${pixmap} $(distdir)/pixmaps; \
	    fi \
	  done \
	fi

distclean-local:
	-rm -rf autom4te.cache || rmdir autom4te.cache

ACLOCAL_AMFLAGS = -I m4 --install

EXTRA_DIST = build-aux/config.rpath
