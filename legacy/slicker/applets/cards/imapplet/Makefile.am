kde_module_LTLIBRARIES = libslickerimapplet.la

INCLUDES = -I$(srcdir)/../../../ $(all_includes)

libslickerimapplet_la_LDFLAGS = $(all_libraries) -avoid-version $(QT_LDFLAGS) $(KDE_RPATH) -module -no-undefineda
libslickerimapplet_la_LIBADD  = $(top_builddir)/slicker/carddeskcore/libcarddeskcore.la
libslickerimapplet_la_METASOURCES = AUTO

libslickerimapplet_la_SOURCES = IMApplet.cpp

noinst_HEADERS = IMApplet.h

data_DATA = imapplet.desktop
datadir = $(kde_datadir)/slicker/cardapplets
