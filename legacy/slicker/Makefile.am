INCLUDES = $(all_includes)

SUBDIRS = share carddeskcore cardhandler taskbar core applets slider . 

bin_PROGRAMS = slicker
lib_LTLIBRARIES = slicker.la libslicker_main.la

CLEANFILES = dummy_slicker.cpp dummy_slicker_la.cpp dummy_libslicker_main_la.cpp

libslicker_main_la_LIBADD = share/libslicker_share.la carddeskcore/libcarddeskcore.la cardhandler/libslicker_cardhandler.la taskbar/libslicker_taskbar.la slider/libslicker_slider.la core/libslicker_core.la
libslicker_main_la_SOURCES = dummy_libslicker_main_la.cpp
libslicker_main_la_LDFLAGS = -no-undefined -avoid-version $(all_libraries)

slicker_la_LIBADD  = libslicker_main.la
slicker_la_SOURCES = dummy_slicker_la.cpp
slicker_la_LDFLAGS = $(KDE_PLUGIN) -no-undefined -module $(all_libraries)

slicker_LDADD = libslicker_main.la
slicker_SOURCES = dummy_slicker.cpp
slicker_LDFLAGS = $(all_libraries) $(KDE_RPATH)

dummy_slicker.cpp dummy_slicker_la.cpp dummy_libslicker_main_la.cpp:
	echo > $@
