kde_module_LTLIBRARIES = liblaunchercardapplet.la

INCLUDES = -I$(srcdir)/../../../ -I../../../share $(all_includes)


SUBDIRS =

liblaunchercardapplet_la_LDFLAGS = $(all_libraries)   -version-info 0:0:0 -no-undefined
liblaunchercardapplet_la_LIBADD = $(top_builddir)/slicker/carddeskcore/libcarddeskcore.la -lkonq
liblaunchercardapplet_la_METASOURCES = AUTO

liblaunchercardapplet_la_SOURCES = launchercardapplet.cpp plugin.cpp kdiconview.cc launcher.cpp krootwm.cc


EXTRA_DIST = plugin.cpp launchercardapplet.cpp kdiconview.cpp krootwm.cpp launchercardapplet.desktop

install-data-local:
	$(mkinstalldirs) $(kde_datadir)/slicker/cardapplets/
	$(INSTALL_DATA) $(srcdir)/launchercardapplet.desktop $(kde_datadir)/slicker/cardapplets/launcherardapplet.desktop

uninstall-local:
	-rm -f $(kde_datadir)/slicker/cardapplets/launchercardapplet.desktop
