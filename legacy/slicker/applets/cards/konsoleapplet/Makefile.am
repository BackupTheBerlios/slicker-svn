kde_module_LTLIBRARIES = libkonsoleapplet.la

INCLUDES = -I../../../ $(all_includes)

libkonsoleapplet_la_LDFLAGS = $(all_libraries) -avoid-version $(QT_LDFLAGS) $(KDE_RPATH) -module
libkonsoleapplet_la_LIBADD  = $(top_builddir)/slicker/carddeskcore/libcarddeskcore.la
libkonsoleapplet_la_METASOURCES = AUTO

libkonsoleapplet_la_SOURCES = KonsoleApplet.cpp plugin.cpp

noinst_HEADERS = KonsoleApplet.h

data_DATA = konsoleapplet.desktop
datadir = $(kde_datadir)/slicker/cardapplets
