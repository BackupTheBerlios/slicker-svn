INCLUDES = -I$(srcdir)/../carddeskcore -I$(srcdir)/../share -I$(srcdir)/../core -I$(srcdir)/../ $(all_includes)

noinst_LTLIBRARIES = libslicker_cardhandler.la

libslicker_cardhandler_la_SOURCES = manager.cpp cardhandler.cpp pluginlistbox.cpp \
	newcarddialog.cpp TestGui.cpp CardHandlerPreferencesUI.ui CardHandlerPreferences.cpp

libslicker_cardhandler_la_LDFLAGS = $(all_libraries) ../carddeskcore/libcarddeskcore.la
libslicker_cardhandler_la_LIBADD =  $(LIB_KDEUI)
libslicker_cardhandler_la_METASOURCES = AUTO

noinst_HEADERS = CardHandlerPreferences.h CardHandlerPreferencesUI.h TestGui.h \
	cardhandler.h manager.h newcarddialog.h pluginlistbox.h
