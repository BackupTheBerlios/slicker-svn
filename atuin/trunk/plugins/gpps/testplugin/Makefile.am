INCLUDES = -I$(top_srcdir)/share $(all_includes)
METASOURCES = AUTO

kde_module_LTLIBRARIES = slicker_testplugin.la kcm_slicker_testplugin.la

slicker_testplugin_la_SOURCES = testplugin.cpp
slicker_testplugin_la_LDFLAGS = -module $(KDE_PLUGIN)
slicker_testplugin_la_LIBADD = $(top_builddir)/share/libslicker_share.la $(top_builddir)/base/libslicker_base.la

kcm_slicker_testplugin_la_SOURCES = testpluginprefs.cpp
kcm_slicker_testplugin_la_LDFLAGS = -module $(KDE_PLUGIN) $(all_libraries)
kcm_slicker_testplugin_la_LIBADD = $(LIB_KDEUI) $(top_builddir)/share/libslicker_share.la

servicedir = $(kde_servicesdir)
service_DATA = slicker_testplugin.desktop
noinst_HEADERS = testplugin.h testpluginprefs.h

kcm_DATA = slicker_testplugin_config.desktop
kcmdir = $(kde_servicesdir)/kconfiguredialog

