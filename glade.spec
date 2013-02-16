%define name    glade2
%define ver      1.1.3
%define  RELEASE SNAP
%define  rel     %{?CUSTOM_RELEASE} %{!?CUSTOM_RELEASE:%RELEASE}
%define prefix   /usr
%define sysconfdir	/etc
%define skreq           0.1.4

Summary: GUI builder for GTK+ and GNOME
Name: %{name}
Version: %ver
Release: %rel
Copyright: GPL
Group: Development/Tools
Source: glade-%{ver}.tar.gz
BuildRoot: /var/tmp/glade-%{ver}-root
URL: http://glade.gnome.org/
Docdir: %{prefix}/doc
Prefix: %prefix
BuildRequires: scrollkeeper >= %skreq

%description
Glade is a GUI builder for GTK+ and GNOME.

%prep
%setup -q -n glade-%{ver}

%ifarch alpha
  ARCH_FLAGS="--host=alpha-redhat-linux"
%endif

if [ ! -f configure ]; then
  CFLAGS="$RPM_OPT_FLAGS" ./autogen.sh $ARCH_FLAGS --prefix=%{prefix} --sysconfdir=%{sysconfdir}
else
  CFLAGS="$RPM_OPT_FLAGS" ./configure $ARCH_FLAGS --prefix=%{prefix} --sysconfdir=%{sysconfdir}
fi

%build

if [ "$SMP" != "" ]; then
  (make "MAKE=make -k -j $SMP"; exit 0)
  make
else
  make
fi

%install
[ -n "$RPM_BUILD_ROOT" -a "$RPM_BUILD_ROOT" != / ] && rm -rf $RPM_BUILD_ROOT

make prefix=$RPM_BUILD_ROOT%{prefix} PACKAGE_PIXMAPS_DIR=$RPM_BUILD_ROOT%{prefix}/share/pixmaps install

%clean
[ -n "$RPM_BUILD_ROOT" -a "$RPM_BUILD_ROOT" != / ] && rm -rf $RPM_BUILD_ROOT

%files
%defattr(-, root, root)

%doc AUTHORS COPYING ChangeLog NEWS README

%{prefix}/bin/glade-2
%{prefix}/share/glade-2/gtk/*
%{prefix}/share/gnome/help/glade-2/C/glade-faq.*
%{prefix}/share/gnome/help/glade-2/C/glade-turbo-start.*
%{prefix}/share/gnome/help/glade-2/C/glade-user-guide.*
%{prefix}/share/gnome/help/glade-2/C/figures/*
%{prefix}/share/locale/*/LC_MESSAGES/*.mo
%{prefix}/share/omf/glade-2/*
%{prefix}/share/pixmaps/glade-2/*
%{prefix}/share/pixmaps/*.*

%post
if which scrollkeeper-update>/dev/null 2>&1; then scrollkeeper-update; fi

%postun
if which scrollkeeper-update>/dev/null 2>&1; then scrollkeeper-update; fi


%changelog
* Tue Oct 08 2002 Damon Chaplin <damon@kendo.fsnet.co.uk>
- Updated description and URL.

* Fri Sep 20 2002 Dermot Musgrove <dermot.musgrove@virgin.net>
- Changes to find doc files in new place

* Mon May 20 2002 Dermot Musgrove <dermot.musgrove@virgin.net>
- Changes to build glade-2

* Sun Mar 25 2001 Dan Mueth <dan@eazel.com>
- Added ScrollKeeper/OMF stuff

* Thu Jul 22 1999 Herbert Valerio Riedel <hvr@hvrlab.dhs.org>
- changed configure options in order to build on all alphas

* Wed Jun 23 1999 Jose Mercado <jmercado@mit.edu>
- Changed the Source variable to use %{var}.
- Fixed glade.desktop's path so rpm will find it.
