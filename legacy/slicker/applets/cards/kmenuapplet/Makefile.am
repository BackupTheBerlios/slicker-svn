kde_module_LTLIBRARIES = libkmenuapplet.la

INCLUDES = -I../../../share $(all_includes)

libkmenuapplet_la_LDFLAGS = $(all_libraries) -avoid-version $(QT_LDFLAGS) $(KDE_RPATH) -module
libkmenuapplet_la_LIBADD  = $(top_builddir)/slicker/carddeskcore/libcarddeskcore.la $(LIB_KICKER)
libkmenuapplet_la_METASOURCES = AUTO

libkmenuapplet_la_SOURCES = kmenuapplet.cpp slickerkmenu.cpp kmenutab.cpp

noinst_HEADERS = kmenuapplet.h kmenutab.h service_mnu.h slickerkmenu.h kmenutab.h

data_DATA = kmenuapplet.desktop
datadir = $(kde_datadir)/slicker/cardapplets
