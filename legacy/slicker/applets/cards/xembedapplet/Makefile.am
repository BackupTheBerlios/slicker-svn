kde_module_LTLIBRARIES = libxembedapplet.la

INCLUDES = -I$(srcdir)/../../../ -I../../../share $(all_includes)

libxembedapplet_la_LDFLAGS = $(all_libraries) -avoid-version $(QT_LDFLAGS) $(KDE_RPATH) -module
libxembedapplet_la_LIBADD  = $(top_builddir)/slicker/carddeskcore/libcarddeskcore.la
libxembedapplet_la_METASOURCES = AUTO

libxembedapplet_la_SOURCES = XEmbedCardApplet.cpp plugin.cpp

noinst_HEADERS = XEmbedCardApplet.h

data_DATA = xembedcardapplet.desktop
datadir = $(kde_datadir)/slicker/cardapplets
