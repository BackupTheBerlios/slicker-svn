METASOURCES = AUTO
INCLUDES = -I$(top_srcdir)/share $(all_includes)

bin_PROGRAMS = slicker
lib_LTLIBRARIES = libslicker_base.la

slicker_SOURCES =
slicker_LDFLAGS = -no-undefined
slicker_LDADD = $(top_builddir)/share/libslicker_share.la $(top_builddir)/base/libslicker_base.la

libslicker_base_la_SOURCES = appletmanager.cpp edgewidgetmanager.cpp pluginmanager.cpp slicker.cpp slider.cpp pluginprefs.cpp sessionmanager.cpp
libslicker_base_la_LIBADD = $(top_builddir)/share/libslicker_share.la -lkutils
libslicker_base_la_LDFLAGS = $(all_libraries)

applnkSystem_DATA =  slicker.desktop
applnkSystemdir = $(kde_appsdir)/System

include_HEADERS = edgewidgetmanager.h slicker.h pluginmanager.h appletmanager.h sessionmanager.h
noinst_HEADERS = slider.h pluginprefs.h

