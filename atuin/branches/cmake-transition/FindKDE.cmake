# Locate KDE include paths and libraries - copied and edited from FindQt.cmake

# This module defines
# KDE_INCLUDE_DIR, where to find headers, etc.
# KDE_LIB_PATHS, possible paths on where to find kde/lib
# KDE_LIB_KDECORE, to link against kdecore.
# KDE_LIB_KDEUI, to link against kdeui
# KDE_LIB_KUTILS, to link against kdeui
# KDE_DEFINITIONS, definitions to use when compiling code that uses KDE.
# KDE_FOUND, If false, don't try to use Qt.

FIND_PATH(KDE_INCLUDE_DIR kdeversion.h
  $ENV{KDEDIR}/include
  /usr/local/kde/include
  /usr/local/include
  /usr/include/kde
  /usr/include
  /opt/kde
  )

SET( KDE_LIB_PATHS
  $ENV{KDEDIR}/lib
  /usr/local/kde/lib
  /usr/local/lib
  /usr/lib
  /usr/share/kde/lib
  /opt/kde/lib
  )  

FIND_LIBRARY(KDE_LIB_KDECORE
  NAMES kdecore
  PATHS ${KDE_LIB_PATHS}
  )

FIND_LIBRARY(KDE_LIB_KDEUI
  NAMES kdeui
  PATHS ${KDE_LIB_PATHS}
  )

FIND_LIBRARY(KDE_LIB_KUTILS
  NAMES kutils
  PATHS ${KDE_LIB_PATHS}
  )

IF(KDE_INCLUDE_DIR)
  IF(KDE_LIBRARIES)
    SET( KDE_FOUND "YES" )
    SET( KDE_DEFINITIONS "")
  ENDIF(KDE_LIBRARIES)
ENDIF(KDE_INCLUDE_DIR)


MARK_AS_ADVANCED(
  KDE_INCLUDE_DIR
  KDE_LIB_KDECORE
  KDE_LIB_KDEUI
  KDE_LIB_KUTILS
  )
