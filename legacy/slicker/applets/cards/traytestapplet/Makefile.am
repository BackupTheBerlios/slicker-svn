kde_module_LTLIBRARIES = libtraytestapplet.la

INCLUDES = -I$(srcdir)/../../../ $(all_includes)

libtraytestapplet_la_LDFLAGS = $(all_libraries) -avoid-version $(QT_LDFLAGS) $(KDE_RPATH) -module
libtraytestapplet_la_LIBADD  = $(top_builddir)/slicker/carddeskcore/libcarddeskcore.la
libtraytestapplet_la_METASOURCES = AUTO

libtraytestapplet_la_SOURCES = TrayTestApplet.cpp plugin.cpp

noinst_HEADERS = TrayTestApplet.h

data_DATA = traytestapplet.desktop
datadir = $(kde_datadir)/slicker/cardapplets
