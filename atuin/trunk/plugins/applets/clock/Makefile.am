INCLUDES = -I$(top_srcdir)/share $(all_includes)
METASOURCES = AUTO

kde_module_LTLIBRARIES = slicker_clock.la kcm_slicker_clock.la

slicker_clock_la_SOURCES = clockapplet.cpp clock.cpp
slicker_clock_la_LDFLAGS = -module $(KDE_PLUGIN)
slicker_clock_la_LIBADD = $(top_builddir)/share/libslicker_share.la $(top_builddir)/base/libslicker_base.la

kcm_slicker_clock_la_SOURCES = clockpluginprefs.cpp
kcm_slicker_clock_la_LDFLAGS = -module $(KDE_PLUGIN) $(all_libraries)
kcm_slicker_clock_la_LIBADD = $(top_builddir)/share/libslicker_share.la $(LIB_KIO) $(LIB_KDEUI)

servicedir = $(kde_servicesdir)
service_DATA = slicker_clock.desktop
noinst_HEADERS = clockpluginprefs.h clockapplet.h clock.h

kcm_DATA = slicker_clock_config.desktop
kcmdir = $(kde_servicesdir)/kconfiguredialog

