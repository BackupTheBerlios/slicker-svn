kde_module_LTLIBRARIES = libprinterjobsapplet.la

INCLUDES = -I$(srcdir)/../../../ -I../../../share $(all_includes)

libprinterjobsapplet_la_LDFLAGS = $(all_libraries) -avoid-version $(QT_LDFLAGS) $(KDE_RPATH) -module
libprinterjobsapplet_la_LIBADD  = $(top_builddir)/slicker/carddeskcore/libcarddeskcore.la -lkdeprint_management
libprinterjobsapplet_la_METASOURCES = AUTO

libprinterjobsapplet_la_SOURCES = PrinterJobsApplet.cpp plugin.cpp

noinst_HEADERS = PrinterJobsApplet.h

data_DATA = printerjobsapplet.desktop
datadir = $(kde_datadir)/slicker/cardapplets
