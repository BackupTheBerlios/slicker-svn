lib_LTLIBRARIES = libcarddeskcore.la

INCLUDES = $(all_includes) -I../share

libcarddeskcore_la_LDFLAGS = $(all_libraries) -version-info 0:0:0 $(QT_LDFLAGS) -no-undefined
libcarddeskcore_la_LIBADD  = $(LIB_QT) -lkdecore -lkdefx -lkdeui ../share/libslicker_share.la
libcarddeskcore_la_METASOURCES = AUTO

libcarddeskcore_la_SOURCES = Card.cpp LayoutSwitcher.cpp DragLogic.cpp Deck.cpp Const.cpp CardManager.cpp CardElements.cpp CardApplet.cpp

# FIXME: which of these are public APIs and which aren't?
# anything in include_HEADERS that are private should be moved to noinst_HEADERS

coredir = $(includedir)/carddeskcore
core_HEADERS = CardApplet.h CardElements.h Card.h Const.h
