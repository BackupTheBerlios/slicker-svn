METASOURCES = AUTO
INCLUDES = -I$(top_srcdir)/base $(all_includes)

lib_LTLIBRARIES = libslicker_share.la

libslicker_share_la_SOURCES = edgewidget.cpp edgewidgethelpers.cpp plugin.cpp appletdef.cpp applethost.cpp applet.cpp pluginprefspage.cpp sessionitem.cpp sessionitemfactory.cpp sessionapplet.cpp sessionappletdef.cpp

libslicker_share_la_LDFLAGS = $(LIBSLICKER_SHARE_LDFLAGS) $(all_libraries)
libslicker_share_la_LIBADD = $(LIB_KDEUI)
libslicker_share_la_METASOURCES = AUTO

include_HEADERS = edgewidget.h edgewidgethelpers.h plugin.h applet.h appletdef.h applethost.h pluginprefspage.h sessionitemfactory.h sessionitem.h sessionapplet.h sessionappletdef.h

servicetype_DATA = slickerplugin.desktop
servicetypedir = $(kde_servicetypesdir)
