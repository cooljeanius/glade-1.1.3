## Process this file with automake to produce Makefile.in

SUBDIRS = glade po doc omf-install

Developmentdir = $(datadir)/applications
Development_in_files = glade-2.desktop.in
Development_DATA = $(Development_in_files:.desktop.in=.desktop)

@INTLTOOL_DESKTOP_RULE@

EXTRA_DIST = config.rpath m4/ChangeLog  \
	FAQ \
	glade.spec \
	glade.spec.in \
	glade-2.desktop.in \
	glade-2.png \
	intltool-extract.in \
	intltool-merge.in \
	intltool-update.in \
	examples/editor/AUTHORS \
	examples/editor/ChangeLog \
	examples/editor/Makefile.am \
	examples/editor/NEWS \
	examples/editor/README \
	examples/editor/acconfig.h \
	examples/editor/autogen.sh \
	examples/editor/configure.in \
	examples/editor/editor.glade \
	examples/editor/src/Makefile.am \
	examples/editor/src/callbacks.c \
	examples/editor/src/callbacks.h \
	examples/editor/src/interface.c \
	examples/editor/src/interface.h \
	examples/editor/src/main.c \
	examples/editor/src/support.c \
	examples/editor/src/support.h \
	examples/editor/pixmaps/new.xpm \
	examples/editor/pixmaps/open.xpm \
	examples/editor/pixmaps/save.xpm \
	examples/editor/po/ChangeLog \
	examples/editor/po/POTFILES.in

pixmapsdir = $(datadir)/pixmaps
pixmaps_DATA = glade-2.png

# Install all the Gnome m4 macros we use to build Glade, so we can copy them
# to each project's directory (for Gnome projects).
install-data-local:
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(datadir)/pixmaps/glade-2
	$(INSTALL_DATA) $(srcdir)/glade/graphics/glade_logo.png $(DESTDIR)$(datadir)/pixmaps/glade-2

ACLOCAL_AMFLAGS = -I m4 --install
