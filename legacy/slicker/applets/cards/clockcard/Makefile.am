kde_module_LTLIBRARIES = libclockcardapplet.la

INCLUDES = -I$(srcdir)/../../../ -I../../../share $(all_includes)

libclockcardapplet_la_LDFLAGS = $(all_libraries) -avoid-version $(QT_LDFLAGS) $(KDE_RPATH) -module
libclockcardapplet_la_LIBADD  = $(top_builddir)/slicker/carddeskcore/libcarddeskcore.la
libclockcardapplet_la_METASOURCES = AUTO

libclockcardapplet_la_SOURCES = settings.cpp datepicker.cpp clock.cpp conf.ui

noinst_HEADERS = clock.h conf.h datepicker.h settings.h

data_DATA = clockapplet.desktop
datadir = $(kde_datadir)/slicker/cardapplets
