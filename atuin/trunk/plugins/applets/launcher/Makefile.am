INCLUDES = -I$(top_srcdir)/share $(all_includes)
METASOURCES = AUTO

kde_module_LTLIBRARIES = slicker_launcherplugin.la kcm_slicker_launcherplugin.la

slicker_launcherplugin_la_SOURCES = launcherplugin.cpp launcherapplet.cpp launcherbutton.cpp
slicker_launcherplugin_la_LDFLAGS = -module $(KDE_PLUGIN)
slicker_launcherplugin_la_LIBADD = $(top_builddir)/share/libslicker_share.la $(top_builddir)/base/libslicker_base.la

kcm_slicker_launcherplugin_la_SOURCES = launcherpluginprefs.cpp
kcm_slicker_launcherplugin_la_LDFLAGS = -module $(KDE_PLUGIN) $(all_libraries)
kcm_slicker_launcherplugin_la_LIBADD = $(top_builddir)/share/libslicker_share.la $(LIB_KIO) $(LIB_KDEUI)

servicedir = $(kde_servicesdir)
service_DATA = slicker_launcherplugin.desktop
noinst_HEADERS = launcherplugin.h launcherpluginprefs.h launcherapplet.h launcherbutton.h

kcm_DATA = slicker_launcherplugin_config.desktop
kcmdir = $(kde_servicesdir)/kconfiguredialog

