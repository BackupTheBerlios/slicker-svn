PROJECT(slicker)

INCLUDE( ${CMAKE_ROOT}/Modules/FindQt.cmake )
INCLUDE( FindKDE.cmake )
INCLUDE_DIRECTORIES( ${QT_INCLUDE_DIR} ${KDE_INCLUDE_DIR} )
ADD_DEFINITIONS( ${QT_DEFINITIONS} ${KDE_DEFINITIONS} )
LINK_LIBRARIES( ${QT_LIBRARIES} ${KDE_LIB_KDECORE} ${KDE_LIB_KUTILS} )

INCLUDE_DIRECTORIES( ${slicker_SOURCE_DIR} ${slicker_SOURCE_DIR}/share ${slicker_SOURCE_DIR}/base )
LINK_DIRECTORIES( ${slicker_BINARY_DIR}/base ${slicker_BINARY_DIR}/share )

SET( SLICKER_PLUGINS_DIR /lib/kde3 )
SET( SLICKER_SERVICES_DIR /share/services )

SUBDIRS( share base plugins )

ADD_EXECUTABLE( slicker slicker.cpp sources_moc)

QT_WRAP_CPP( slicker sources_moc 
             slicker.h )

TARGET_LINK_LIBRARIES( slicker slicker_base slicker_share )

INSTALL_TARGETS( /bin slicker )
