INCLUDES = $(all_includes) -I../core

noinst_LTLIBRARIES = libslicker_taskbar.la

libslicker_taskbar_la_SOURCES = taskbar.cpp taskbarview.cpp taskbarsection.cpp preferences.cpp preferencesUI.ui

SUBDIRS = taskmanager .

libslicker_taskbar_la_LDFLAGS = $(all_libraries)
libslicker_taskbar_la_LIBADD =  $(LIB_KDEUI) $(LIB_KICKER) taskmanager/libslicker_tbm.la
METASOURCES = AUTO

noinst_HEADERS = taskbar.h taskbarsection.h taskbarview.h preferences.h
