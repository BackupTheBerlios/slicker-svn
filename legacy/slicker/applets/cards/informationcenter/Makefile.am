kde_module_LTLIBRARIES = libinformationcenterapplet.la

INCLUDES = -I$(srcdir)/../../../ -I$(srcdir)/../../../carddeskcore -I../../../share -I$(kde_includes)/ksgrd $(all_includes)

libinformationcenterapplet_la_LDFLAGS = $(all_libraries) -avoid-version $(QT_LDFLAGS) $(KDE_RPATH) -module $(KDE_PLUGIN)
libinformationcenterapplet_la_LIBADD  = $(top_builddir)/slicker/carddeskcore/libcarddeskcore.la -lkdefakes -lksgrd $(LIB_KDEUI) $(LIB_KIO)
libinformationcenterapplet_la_METASOURCES = AUTO

libinformationcenterapplet_la_SOURCES = informationcenterapplet.cpp informationcenterwidget.cpp diskinfo.cpp \
					baseinfo.cpp plugin.cpp getinfo.cpp configdialog.ui configdialog_impl.cpp \
					disklist.cpp disks.cpp listview.cpp

noinst_HEADERS = informationcenterapplet.h informationcenterwidget.h baseinfo.h getinfo.h configdialog.h \
		 configdialog_impl.h diskinfo.h disklist.h disks.h listview.h

data_DATA = informationcenterapplet.desktop
datadir = $(kde_datadir)/slicker/cardapplets
