SUBDIRS = . test

INCLUDES = -I$(srcdir)/../carddesk -I.. $(all_includes)

noinst_LTLIBRARIES = libslicker_share.la

libslicker_share_la_SOURCES = plugininfo.cpp pluginloader.cpp edgewidget.cpp edgewidgetmanager.cpp edgewidgethelpers.cpp

libslicker_share_la_LDFLAGS = $(LIBSLICKER_SHARE_LDFLAGS) $(all_libraries) -no-undefined
libslicker_share_la_LIBADD =  $(LIB_KDEUI) -lkdecore
libslicker_share_la_METASOURCES = AUTO

noinst_HEADERS = plugininfo.h edgewidget.h edgewidgetmanager.h edgewidgethelpers.h
