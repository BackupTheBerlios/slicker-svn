kde_module_LTLIBRARIES = libtestapplet.la

INCLUDES = -I$(srcdir)/../../../ -I../../../share $(all_includes)

libtestapplet_la_LDFLAGS = $(all_libraries) -avoid-version $(QT_LDFLAGS) $(KDE_RPATH) -module
libtestapplet_la_LIBADD  = $(top_builddir)/slicker/carddeskcore/libcarddeskcore.la
libtestapplet_la_METASOURCES = AUTO

libtestapplet_la_SOURCES = TestCardApplet.cpp plugin.cpp

noinst_HEADERS = TestCardApplet.h

data_DATA = testcardapplet.desktop
datadir = $(kde_datadir)/slicker/cardapplets
