INCLUDES = -I$(srcdir)/../slider -I$(srcdir)/../taskbar -I$(srcdir)/../cardhandler -I$(srcdir)/../carddesk  $(all_includes)

noinst_LTLIBRARIES = libslicker_core.la

libslicker_core_la_SOURCES = main.cpp slicker.cpp pluginmanager.cpp SlickerPreferencesUI.ui \
	SlickerPreferences.cpp PreferencesDialog.cpp ConfigModule.cpp

libslicker_core_la_LDFLAGS = $(all_libraries)
libslicker_core_la_LIBADD =  $(LIB_KDEUI)
libslicker_core_la_METASOURCES = AUTO

# FIXME: which of these are public APIs and which aren't?
# anything in include_HEADERS that are private should be moved to noinst_HEADERS

noinst_HEADERS = ConfigModule.h PreferencesDialog.h SlickerPreferences.h \
	SlickerPreferencesUI.h pluginmanager.h slicker.h
