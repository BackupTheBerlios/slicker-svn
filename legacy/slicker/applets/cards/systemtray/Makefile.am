kde_module_LTLIBRARIES = libsystemtrayapplet.la

INCLUDES = -I$(srcdir)/../../../  -I$(srcdir)/../../../share $(all_includes)

libsystemtrayapplet_la_LDFLAGS = $(all_libraries) -avoid-version $(QT_LDFLAGS) $(KDE_RPATH) -module
libsystemtrayapplet_la_LIBADD  = $(top_builddir)/slicker/carddeskcore/libcarddeskcore.la
libsystemtrayapplet_la_METASOURCES = AUTO

libsystemtrayapplet_la_SOURCES = systemtraywidget.cpp contentswidget.cpp systemtray.cpp \
				 plugin.cpp configure_ui.ui configure_impl.cpp

noinst_HEADERS = systemtraywidget.h systemtray.h configure_impl.h contentswidget.h

data_DATA = systemtrayapplet.desktop
datadir = $(kde_datadir)/slicker/cardapplets
